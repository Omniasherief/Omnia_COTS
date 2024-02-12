/*
 * ADC_interface.h
 *
 *  Created on: Dec 13, 2023
 *      Author: omnia sherief
 */

#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_



typedef struct
{
	uint8  ChainSize;
	uint8* ChannelArr;
	uint16* ResultArr;
	void(*NotificationFunc)(void);
}ADC_Chain_t;
void ADC_voidInit(void);
uint8 ADC_u8StartSingleConversionSync (uint8 Copy_u8Channel,uint16 * Copy_pu16Result);  /*Result of conversion*/
uint8 ADC_u8StartSingleConversionAsync  (uint8 Copy_u8Channel,uint16 * Copy_pu16Result, void(*Copy_pvNotificationFunc)(void));
uint8 ADC_u8StartChainConversionAsynch(const ADC_Chain_t* Copy_object);



#endif /* ADC_INTERFACE_H_ */
