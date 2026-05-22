/* 
 * File:	eh008_i2c.h
 * 
 * Author: 
 * Created on 21 May 2026
 */

#ifndef __EH008_I2C_H__
#define __EH008_I2C_H__

////////////////////////////////////////////////////////////////////////////////
// application includes
#include "eh008_common.h"

////////////////////////////////////////////////////////////////////////////////
// system includes
#include <stdbool.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// public functions

#ifdef FEAT_I2C
void eh008_mssp1_init(void);
void eh008_i2c_master_start(void);
void eh008_i2c_master_restart(void);
void eh008_i2c_master_stop(void);
uint8_t eh008_i2c_master_write(uint8_t data);
uint8_t eh008_i2c_master_read(bool ack);
#endif // FEAT_I2C

#endif // __EH008_I2C_H__
