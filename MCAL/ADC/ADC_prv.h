/*
 * ADC_prv.h
 *
 *  Created on: Dec 13, 2023
 *      Author: omnia sherief
 */

#ifndef ADC_PRV_H_
#define ADC_PRV_H_

#define DIVISION_BY_128                 7u
#define DIVISION_BY_64	 	            6u
#define DIVISION_BY_32	 	            5u
#define DIVISION_BY_16	 	            4u
#define DIVISION_BY_8                   3u
#define DIVISION_BY_4                   2u
#define DIVISION_BY_2                   1u


#define PRESCALER_BIT_MASK     0b11111000
#define PRESCALER_BIT_POS      0u


#define CHANNEL_BIT_MASK       0b11100000
#define EIGHT_BITS             1u
#define TEN_BITS               2u

#define IDLE                   0u
#define BUSY                   1u


#define  SINGLE_ASYNCH      0
#define  CHAIN_ASYNCH       1


static void voidHandleSingleConvAsynch(void);

static void voidHandleChainConvAsynch(void);
#endif /* ADC_PRV_H_ */
