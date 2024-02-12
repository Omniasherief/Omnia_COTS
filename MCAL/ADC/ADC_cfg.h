/*
 * ADC_cfg.h
 *
 *  Created on: Dec 13, 2023
 *      Author: omnia sherief
 */

#ifndef ADC_CFG_H_
#define ADC_CFG_H_

/*Configure the ADC prescaler division factor ,
 * option 1- division by 2
 *2- division by 4
 *3- division by 8
 *4- division by 16
 *5- division by 32
 *6- division by 64
 *7- division by 128 */

#define ADC_u8PRESCALER_DIVISION_FACTOR    DIVISION_BY_128

/*configure the ADC Resolution 1- EIGHT_BITS
 *                             2- TEN_BITS
 * */
#define ADC_u8Resolution   TEN_BITS
#define ADC_u32TIMEOUT     50000u
#endif /* ADC_CFG_H_ */
