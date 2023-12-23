/*
 * ADC_prog.c
 *
 *  Created on: Dec 17, 2023
 *      Author: omnia sherief
 */


#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "ErrType.h"

#include "ADC_reg.h"
#include "ADC_prv.h"
#include"ADC_cfg.h"
#include "ADC_interface.h"

void ADC_voidInit(void)
{

/*Set AVCC as Refrence Voltage*/
SET_BIT(ADMUX,ADMUX_REFS0 );
CLR_BIT(ADMUX,ADMUX_REFS1);

/* select left or right adjust According to Resolution Configuration */
#if  ADC_u8Resolution ==EIGHT_BITS
SET_BIT(ADMUX,ADMUX_ADLAR);
#elif ADC_u8Resolution ==TEN_BITS
CLR_BIT(ADMUX,ADMUX_ADLAR);
#else
#error wrong ADC_u8Resolution Configuration Option
#endif
/*DISABLE AUTO Trigger Mode */
CLR_BIT(ADCSRA,ADCSRA_ADATE);

/*DISABLE Interrupt Enable  */
CLR_BIT(ADCSRA,ADCSRA_ADIE);

/*ENABLE ADC*/
SET_BIT(ADCSRA,ADCSRA_ADEN);

/*configure the prescaler division */

/*Clear prescaler*/
ADCSRA &= PRESCALER_BIT_MASK;
ADCSRA |= ADC_u8PRESCALER_DIVISION_FACTOR << PRESCALER_BIT_POS ;

}
uint8 ADC_u8GetChannelReading (uint8 Copy_u8Channel)
 {
    /*Set the input channel*/
	ADMUX &= CHANNEL_BIT_MASK;   /*clear the channel selection bits*/
	ADMUX |= Copy_u8Channel;
    /*Start Conversion*/
   SET_BIT(ADCSRA,ADCSRA_ADSC);
   /*Polling (WITH BLOCKING) on the conversion complete*/
   while(GET_BIT(ADCSRA,ADCSRA_ADIF)==0);     //(!GET_BIT(ADCSRA,ADCSRA_ADIF))

   /*Clear Conversion complete*/
    SET_BIT(ADCSRA,ADCSRA_ADIF);

   /*get conversion result*/
    return ADCH;
 }
