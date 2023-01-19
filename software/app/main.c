#include "system.h"
#include "sys/alt_stdio.h"
#include "alt_types.h"
#include "io.h"
#include "unistd.h"
#include "sys/alt_sys_init.h"
#include "opencores_i2c.h"
#include "opencores_i2c_regs.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"

// les adresses de registres de ADXL345
#define ADXL345_DATAX0 0x32
#define ADXL345_DATAX1 0x33
#define ADXL345_DATAY0 0x34
#define ADXL345_DATAY1 0x35
#define ADXL345_DATAZ0 0x36
#define ADXL345_DATAZ1 0x37
#define ADXL345_OFSX   0x1E
#define ADXL345_OFSY   0x1F
#define ADXL345_OFSZ   0x20
#define DATA_FORMAT    0x37
#define ACT_INACT_CTL  0x27
#define POWER_CT       0x2D

// les variables pour stocker les valeurs de X, Y et Z, et switch
alt_16 X,Y,Z, data[6], switch_value;

// les fonctions pour lire et ecrire dans les registres
void write_register(alt_16 x,int value);
int read_register(alt_16 x);
alt_32 int_to_bcd(alt_32 int_value);

// fonction pour lire et afficher les valeurs des axes
void axes();

void write_register(alt_16 x, int value) {
  I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 0);
  I2C_write(OPENCORES_I2C_0_BASE, x, 0);
  I2C_write(OPENCORES_I2C_0_BASE, value, 1);
}

int read_register(alt_16 x) {
  I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 0);
  I2C_write(OPENCORES_I2C_0_BASE, x, 0);
  I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);
  return I2C_read(OPENCORES_I2C_0_BASE, 1);
}

alt_32 int_to_bcd(alt_32 int_value) {
  alt_32 result = 0;
  if (int_value < 0) {
    int_value = -int_value;
    result = 15 << 16;
  } else {
    result = 14 << 16;
  }
  char bcd_value[4];
  bcd_value[0] = int_value % 10;
  bcd_value[1] = int_value / 10 % 10;
  bcd_value[2] = int_value / 100 % 10;
  bcd_value[3] = int_value / 1000 % 10;
  return result = result + (bcd_value[3] << 12) + (bcd_value[2] << 8) + (bcd_value[1] << 4) + bcd_value[0];
}

void axes() {
  switch (switch_value) {
  case 0b00:
    IOWR_ALTERA_AVALON_PIO_DATA(SEG_BASE, (10 << 20) + int_to_bcd(X * 4));
    printf("Valeur pour switch 00: x\n");
    break;
  case 0b01:
    IOWR_ALTERA_AVALON_PIO_DATA(SEG_BASE, (11 << 20) + int_to_bcd(Y * 4));
    printf("Valeur pour switch 01: y\n");
    break;
  case 0b10:
    IOWR_ALTERA_AVALON_PIO_DATA(SEG_BASE, (12 << 20) + int_to_bcd(Z * 4));
    printf("Valeur pour switch 10: z\n");
    break;
  }
}

static void timer_interrupt(void * Context, alt_u32 id);
static void irqhandler_switch(void * context);

int main() {
  // Initialisation de la communication I2C
  I2C_init(OPENCORES_I2C_0_BASE, ALT_CPU_FREQ, 100000);

  // Vérification de la connexion avec la périphérique I2C
  if (I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 0) == 0) {
    printf("connection établie\n");
  } else {
    printf("error de connexion\n");
  }
  // Initialisation et configuration du timer pour les interruptions
  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
  alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, (void * ) timer_interrupt, NULL, 0);
  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0x07);

  // Initialisation et configuration des commutateurs pour les interruptions
  IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SWITCH_BASE, 0b1111);
  IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCH_BASE, 0x0);
  alt_ic_isr_register(SWITCH_IRQ_INTERRUPT_CONTROLLER_ID, SWITCH_IRQ, (void * ) irqhandler_switch, NULL, NULL);

  // Ecriture dans les registres pour configurer l'acceleromètre
  write_register(ACT_INACT_CTL, 240);
  write_register(POWER_CT, 8);
  write_register(DATA_FORMAT, 8);
  write_register(ADXL345_OFSX, 1);
  write_register(ADXL345_OFSY, 0);
  write_register(ADXL345_OFSZ, 2);

  // Lecture de la valeur des commutateurs
  switch_value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);

  while (1);
  return 0;
}

static void timer_interrupt(void * Context, alt_u32 id) {
  // Lecture des données de l'acceleromètre
  data[0] = read_register(ADXL345_DATAX0);
  data[1] = read_register(ADXL345_DATAX1);
  // combiner les données X
  X = (data[1] << 8) | data[0];

  data[2] = read_register(ADXL345_DATAY0);
  data[3] = read_register(ADXL345_DATAY1);
  // combiner les données Y
  Y = (data[3] << 8) | data[2];

  data[4] = read_register(ADXL345_DATAZ0);
  data[5] = read_register(ADXL345_DATAZ1);
  // combiner les données Z
  Z = (data[5] << 8) | data[4];

  // Affichage des valeurs X, Y, Z dans la console
  printf("X: %d Y: %d Z: %d \n", X * 4, Y * 4, Z * 4);

  // Appel de la fonction pour afficher les valeurs de X, Y, Z sur les afficheurs 7 segments en fonction de la position des switchs
  axes();

  // Réinitialisation du statut du timer
  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
}

static void irqhandler_switch(void * context) {
  switch_value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
  IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCH_BASE, 0x0);
}