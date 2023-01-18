/*
 * Lab3_definitions.c
 *
 *  Created on: 13 janv. 2023
 *      Author: Ahmed
 */

#include "Lab3_definitions.h"

void write_register(alt_16 x,int value) {
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



alt_32 int_to_bcd(alt_32 int_value)
{
	alt_32 result=0;
    if (int_value < 0) {
        int_value = -int_value;
        result=15<<16;
    }
    else{ result=14<<16;}
    char bcd_value[4];
    bcd_value[0] = int_value % 10;
    bcd_value[1] = int_value / 10 % 10;
    bcd_value[2] = int_value / 100 % 10;
    bcd_value[3] = int_value / 1000 % 10;
    return result=result+(bcd_value[3] << 12)+(bcd_value[2] << 8) + (bcd_value[1] << 4) + bcd_value[0];
}

void axes() {
    switch(switch_value) {
        case 0b00:
        	  IOWR_ALTERA_AVALON_PIO_DATA(SEG_BASE, (10 << 20)+int_to_bcd(X * 4));
            printf("Valeur pour switch 00: x\n");
            break;
        case 0b01:
        	  IOWR_ALTERA_AVALON_PIO_DATA(SEG_BASE, (11 << 20)+int_to_bcd(Y * 4));
            printf("Valeur pour switch 01: y\n");
            break;
        case 0b10:
        	  IOWR_ALTERA_AVALON_PIO_DATA(SEG_BASE, (12 << 20)+int_to_bcd(Z * 4));
            printf("Valeur pour switch 10: z\n");
            break;}
}
