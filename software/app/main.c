#include "Lab3_definitions.h"


static void timer_interrupt(void * Context, alt_u32 id);
static void irqhandler_switch(void * context);

int main() {

  I2C_init(OPENCORES_I2C_0_BASE, ALT_CPU_FREQ, 100000);

  if (I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 0) == 0) {
    printf("connection établie\n");
  } else {
    printf("error de connexion\n");
  }

  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
  alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, (void * ) timer_interrupt, NULL, 0);
  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0x07);

  IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SWITCH_BASE, 0b1111);
  IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCH_BASE, 0x0);
  alt_ic_isr_register(SWITCH_IRQ_INTERRUPT_CONTROLLER_ID, SWITCH_IRQ, (void * ) irqhandler_switch, NULL, NULL);

  write_register(ACT_INACT_CTL, 240);
  write_register(POWER_CT, 8);
  write_register(DATA_FORMAT, 8);
  write_register(ADXL345_OFSX, 0);
  write_register(ADXL345_OFSY, -1);
  write_register(ADXL345_OFSZ, 0);

  switch_value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);

  while (1);
  return 0;
}

static void timer_interrupt(void * Context, alt_u32 id) {

  data[0] = read_register(ADXL345_DATAX0);

  data[1] = read_register(ADXL345_DATAX1);

  X = (data[1] << 8) | data[0];

  data[2] = read_register(ADXL345_DATAY0);

  data[3] = read_register(ADXL345_DATAY1);

  Y = (data[3] << 8) | data[2];

  data[4] = read_register(ADXL345_DATAZ0);

  data[5] = read_register(ADXL345_DATAZ1);

  Z = (data[5] << 8) | data[4];

  printf("X: %d Y: %d Z: %d \n", X * 4, Y * 4, Z * 4);
  //printf("OFFSETX : %d OFFSETY : %d OFFSETZ : %d \n", read_register(ADXL345_OFSX),read_register(ADXL345_OFSY),read_register(ADXL345_OFSZ));

  axes();
  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
}

static void irqhandler_switch(void * context) {
  switch_value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
  IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCH_BASE, 0x0);
}
