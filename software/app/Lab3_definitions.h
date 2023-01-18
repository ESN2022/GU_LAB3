/*
 * Lab3_definitions.h
 *
 *  Created on: 17 janv. 2023
 *      Author: Ahmed
 */

#include <stdio.h>
#include "sys/alt_stdio.h"
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "sys/alt_sys_init.h"
#include "opencores_i2c.h"


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


alt_16 X,Y,Z, data[6], switch_value;
void write_register(alt_16 x,int value);
int read_register(alt_16 x);
alt_32 int_to_bcd(alt_32 int_value);
void axes();
