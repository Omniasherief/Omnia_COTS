/*
 * ADC_reg.h
 *
 *  Created on: Dec 13, 2023
 *      Author: omnia sherief
 */

#ifndef ADC_REG_H_
#define ADC_REG_H_




#define ADMUX                  *((volatile uint8 *)0x27) /*ADC MULTIPLIXER Selction  Register  */
#define ADMUX_REFS1             7u /* Refrence selecttion bit 1 */
#define ADMUX_REFS0             6u /*  Refrence selecttion bit 0 */
#define ADMUX_ADLAR             5u  /* left Ajust result */

#define ADCSRA                  *((volatile uint8 *)0x26)   /*ADC Control and status register  */
#define ADCSRA_ADEN              7u  /*Enable */
#define ADCSRA_ADSC              6u  /* Start Conversion*/
#define ADCSRA_ADATE             5u  /*Auto trigger Enable */
#define ADCSRA_ADIF              4u  /* Interrupt flag*/
#define ADCSRA_ADIE              3u  /*Interrupt Enable */

#define ADCH                    *((volatile uint8 *)0x25)  /*ADC data high Register  */
#define ADCL                    *((volatile uint8*)0x24)  /*ADC data Low Register  */
#define ADCLP                     *((volatile uint16*)0x24)



#endif /* ADC_REG_H_ */
