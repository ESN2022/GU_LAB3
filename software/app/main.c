/***************************** Fiches En tête *******************************/
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


/***************************** define Registre et Valeur *******************************/
// Addresse
#define ADDR 0x1d

// Registre control et format
#define ACT_INACT_CTL 0X27
#define POWER_CTL 0x2D
#define DATA_FORMAT 0x31

// DATA Registre X,Y,Z
#define ADXL345_DATAX0 0x32
#define ADXL345_DATAX1 0x33
#define ADXL345_DATAY0 0x34
#define ADXL345_DATAY1 0x35
#define ADXL345_DATAZ0 0x36
#define ADXL345_DATAZ1 0x37

// Registre OFFSET => Valeur d'offset
#define ADXL345_OFSX 0x1E
#define ADXL345_OFSY 0x1F
#define ADXL345_OFSZ 0x20

#define CALIBX 0
#define CALIBY 2
#define CALIBZ 18
#define SPEED 100000			// Pour I2C Initialisation


int Tmp0,Tmp1;

/***************************** Fonction Lecture Ecriture *******************************/

int lecture_i2c(int base,int addr){
	
	int data = 0;
	I2C_start(base,ADDR, 0);
	I2C_write(base,addr,0);
	
	I2C_start(base,ADDR,1);
	data = I2C_read(base,1);
	
	return data;
}

void ecriture_i2c(int base,int addr,int value){
	I2C_start(base, ADDR, 0);
	I2C_write(base,addr,0);
	I2C_write(base,value,1);
}


/***************************** Fonction int => SEG & Affichage SEG*******************************/

void affichage_SEG(int Tmp0,int Tmp1){
	int a0,a1,a;
	a0 = lecture_i2c(OPENCORES_I2C_0_BASE,Tmp0);
	a1 = lecture_i2c(OPENCORES_I2C_0_BASE,Tmp1);
	a = (a1<<8)|a0;
	a =	(short) a;
	a = a * 4;
	int2seg(a);
}

void int2seg(int value_int){
	int i,j,k,l,m,signe = 0;
	
	// Signes + / -
	if(value_int < 0){
		signe = 0b00111111;
		value_int = - value_int;
	}
	else
		signe = 0b01000000;	

	
	i = value_int % 10;				// digit
	j = value_int/10 % 10;			// 10 digit
	k = value_int/100 % 10;			// 100 digit
	l = value_int/1000 % 10;		// 1000 digit
	m = value_int/10000 % 10;		// 10000 digit

	IOWR_ALTERA_AVALON_PIO_DATA(SEG0_BASE,i);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG1_BASE,j);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG2_BASE,k);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG3_BASE,l);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG4_BASE,m);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG5_BASE,signe);
}

// Calibration X,Y,Z
void calibration(){
	ecriture_i2c(OPENCORES_I2C_0_BASE,ADXL345_OFSX,CALIBX);
	usleep(100000);
	ecriture_i2c(OPENCORES_I2C_0_BASE,ADXL345_OFSY,CALIBY);
	usleep(100000);
	ecriture_i2c(OPENCORES_I2C_0_BASE,ADXL345_OFSZ,CALIBZ);
	usleep(100000);
}

void affichache_UART(){
	int x1,x0,y1,y0,z1,z0;

	x0 = lecture_i2c(OPENCORES_I2C_0_BASE,ADXL345_DATAX0);
	x1 = lecture_i2c(OPENCORES_I2C_0_BASE,ADXL345_DATAX1);
	y0 = lecture_i2c(OPENCORES_I2C_0_BASE,ADXL345_DATAY0);
	y1 = lecture_i2c(OPENCORES_I2C_0_BASE,ADXL345_DATAY1);
	z0 = lecture_i2c(OPENCORES_I2C_0_BASE,ADXL345_DATAZ0);
	z1 = lecture_i2c(OPENCORES_I2C_0_BASE,ADXL345_DATAZ1);
	
	alt_printf("X= %x, Y= %x, Z= %x\n",(x1<<8)|x0,(y1<<8)|y0,(z1<<8)|z0);

}

// Initialisation du ADXL345
void init_ADXL345(){
	alt_printf("Initialisation I2C\n");
	I2C_init(OPENCORES_I2C_0_BASE,ALT_CPU_FREQ,SPEED);
	if(I2C_start(OPENCORES_I2C_0_BASE,ADDR,0)== 0){
		alt_printf("Init ok\n");
	}
	// POWER_CTL
	alt_printf("Ecriture 0X08 -> POWER_CTL\n");
	ecriture_i2c(OPENCORES_I2C_0_BASE,POWER_CTL,0x08);
	usleep(100000);
	alt_printf("Lecture POWER_CTL\n");
	alt_printf("POWER_CTL = %x\n",lecture_i2c(OPENCORES_I2C_0_BASE,POWER_CTL));
	// DATA_FORMAT	
	alt_printf("Ecriture 0x07 -> DATA_FORMAT\n");
	ecriture_i2c(OPENCORES_I2C_0_BASE,DATA_FORMAT,0x07);
	usleep(100000);
	// DATA_FORMAT
	alt_printf("Lecture DATA_FORMAT\n");
	alt_printf("DATA_FORMAT = %x\n\n",lecture_i2c(OPENCORES_I2C_0_BASE,DATA_FORMAT));
	
	Tmp0 = ADXL345_DATAX0;
	Tmp1 = ADXL345_DATAX1;
}
/***************************** Interruption *******************************/
// Interruption PUSH
static void push_interrupt(void *Context, alt_u32 id){
	switch(Tmp0){
		case ADXL345_DATAX0:
			alt_printf("switch sur Y\n\n");
			Tmp0 = ADXL345_DATAY0;
			Tmp1 = ADXL345_DATAY1;
			break;
		case ADXL345_DATAY0:
			alt_printf("switch sur Z\n\n");
			Tmp0 = ADXL345_DATAZ0;
			Tmp1 = ADXL345_DATAZ1;
			break;
		case ADXL345_DATAZ0:
			alt_printf("switch sur X\n\n");
			Tmp0 = ADXL345_DATAX0;
			Tmp1 = ADXL345_DATAX1;
			break;
		default:
			alt_printf("switch sur Z\n\n");
			Tmp0 = ADXL345_DATAX0;
			Tmp1 = ADXL345_DATAX1;
			break;
	}	
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSH_BASE,0b1);
}

// Initialisation_interruption_push
void init_push_interrupt(){
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PUSH_BASE,0b1);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSH_BASE,0b1);	
	if(alt_irq_register(PUSH_IRQ,NULL,push_interrupt) != 0){
		alt_printf("Erreur interruption push\n");
}
}
// Interruption TIMER et Affichage
static void timer_interrupt(void *Context, alt_u32 id){

	affichache_UART();
	affichage_SEG(Tmp0,Tmp1);
	
	// Effacer l'interruption
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0b01);
}

// Initialisation_interruption_timer
void init_timer_interrupt(){
	if(alt_irq_register(TIMER_0_IRQ,NULL, timer_interrupt) != 0){
		alt_printf("Erreur interruption timer\n");
	}
}
/******************************************** main ****************************************************/
int main(){	
	init_ADXL345();
	calibration();
	init_push_interrupt();
	init_timer_interrupt();
}
