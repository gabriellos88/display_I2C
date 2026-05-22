/* 
 * File:	eh008_i2c.c
 * 
 * Author: 
 * Created on 21 May 2026
 */

////////////////////////////////////////////////////////////////////////////////
// application includes
#include "eh008_i2c.h"

#ifdef FEAT_I2C

////////////////////////////////////////////////////////////////////////////////
// private functions

//------------------------------------------------------------------------------
static void eh008_i2c_wait_idle(void)
//------------------------------------------------------------------------------
{
	while ((SSP1CON2 & 0x1F) || (SSP1STAT & 0x04)) {
	}
}

////////////////////////////////////////////////////////////////////////////////
// public functions

//------------------------------------------------------------------------------
void eh008_mssp1_init(void)
//------------------------------------------------------------------------------
{
	// analog / digital
	ANSELCbits.ANSC3 = ANSEL_DIGITAL; // SCL
	ANSELCbits.ANSC4 = ANSEL_DIGITAL; // SDA

	// direction: MSSP I2C uses open-drain style lines released as inputs.
	TRISCbits.TRISC3 = GPIO_INPUT; // SCL
	TRISCbits.TRISC4 = GPIO_INPUT; // SDA

	SSP1CON1 = 0x00;
	SSP1CON2 = 0x00;

	// Slew-rate disabled for 100 kHz standard mode.
	SSP1STATbits.SMP = 1;

	// I2C master mode, clock = Fosc / (4 * (SSP1ADD + 1)).
	SSP1ADD = (uint8_t)((_XTAL_FREQ / (4UL * EH008_I2C1_SPEED_HZ)) - 1UL);

	// Enable MSSP1.
	SSP1CON1bits.SSPM = 0b1000;
	SSP1CON1bits.SSPEN = 1;

	PIR1bits.SSP1IF = 0;
}

//------------------------------------------------------------------------------
void eh008_i2c_master_start(void)
//------------------------------------------------------------------------------
{
	eh008_i2c_wait_idle();
	PIR1bits.SSP1IF = 0;
	SSP1CON2bits.SEN = 1;
	while (PIR1bits.SSP1IF == 0) {
	}
	PIR1bits.SSP1IF = 0;
}

//------------------------------------------------------------------------------
void eh008_i2c_master_restart(void)
//------------------------------------------------------------------------------
{
	eh008_i2c_wait_idle();
	PIR1bits.SSP1IF = 0;
	SSP1CON2bits.RSEN = 1;
	while (PIR1bits.SSP1IF == 0) {
	}
	PIR1bits.SSP1IF = 0;
}

//------------------------------------------------------------------------------
void eh008_i2c_master_stop(void)
//------------------------------------------------------------------------------
{
	eh008_i2c_wait_idle();
	PIR1bits.SSP1IF = 0;
	SSP1CON2bits.PEN = 1;
	while (PIR1bits.SSP1IF == 0) {
	}
	PIR1bits.SSP1IF = 0;
}

//------------------------------------------------------------------------------
uint8_t eh008_i2c_master_write(uint8_t data)
//------------------------------------------------------------------------------
{
	eh008_i2c_wait_idle();
	PIR1bits.SSP1IF = 0;
	SSP1BUF = data;
	while (PIR1bits.SSP1IF == 0) {
	}
	PIR1bits.SSP1IF = 0;

	return SSP1CON2bits.ACKSTAT;
}

//------------------------------------------------------------------------------
uint8_t eh008_i2c_master_read(bool ack)
//------------------------------------------------------------------------------
{
	uint8_t data;

	eh008_i2c_wait_idle();
	PIR1bits.SSP1IF = 0;
	SSP1CON2bits.RCEN = 1;
	while (PIR1bits.SSP1IF == 0) {
	}
	PIR1bits.SSP1IF = 0;

	data = SSP1BUF;

	SSP1CON2bits.ACKDT = ack;
	SSP1CON2bits.ACKEN = 1;
	while (PIR1bits.SSP1IF == 0) {
	}
	PIR1bits.SSP1IF = 0;

	return data;
}

#endif // FEAT_I2C
