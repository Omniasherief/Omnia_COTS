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

static uint16* ADC_pu16Result =NULL;

static void(*ADC_pvCallBAckNotificationFunc)(void)=NULL;

static uint8 ADC_u8BusyFlag =   IDLE;

static ADC_Chain_t* ADC_pChainData = NULL;

static uint8 ADC_u8ISRSource;

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
uint8 ADC_u8StartSingleConversionSync (uint8 Copy_u8Channel,uint16* Copy_pu16Result)
 {
	uint32 Local_u32Counter =0u;

	uint8 Local_u8ErrorState= OK;
	if (Copy_pu16Result != NULL)
	{


            /*Check if ADC is IDLE*/
		   if(ADC_u8BusyFlag==IDLE)
		  {


			/*ADC is now Busy*/
			ADC_u8BusyFlag=BUSY;

		   /*Set the input channel*/
			ADMUX &= CHANNEL_BIT_MASK;   /*clear the channel selection bits*/
			ADMUX |= Copy_u8Channel;
		    /*Start Conversion*/
		   SET_BIT(ADCSRA,ADCSRA_ADSC);
		   /*Polling (WITH BLOCKING) on the conversion complete on condition that Time out is not passed yet*/
		   while((GET_BIT(ADCSRA,ADCSRA_ADIF)==0u )&& (Local_u32Counter< ADC_u32TIMEOUT))    //(!GET_BIT(ADCSRA,ADCSRA_ADIF))
		   {
			   Local_u32Counter++;
		   }
		   if(Local_u32Counter==ADC_u32TIMEOUT)
		   {
              /*Time out is passed Without the flag being raised*/
			   Local_u8ErrorState=TIMEOUT_ERR ;
		   }
		     else
		      {   /*flag is raised before Timeout */
			       /*Clear Conversion complete*/
			 		    SET_BIT(ADCSRA,ADCSRA_ADIF);

			 		   /*get conversion result*/
			 		#if  ADC_u8Resolution ==EIGHT_BITS
			 		   *Copy_pu16Result= ADCH;
			 		#elif ADC_u8Resolution ==TEN_BITS
			 		  *Copy_pu16Result= ADC;
			 		#endif

		       }
		                   /*ADC is now IDLE*/
		 		   			ADC_u8BusyFlag=IDLE;
		  }
		   else {

			   /*ADC is busy,update error state*/
			   Local_u8ErrorState=BUSY_ERR;

		   }
		   /*ADC is now IDLE*/
		   			ADC_u8BusyFlag=IDLE;
	}
	else
	{




		Local_u8ErrorState=NULL_PTR_ERR;
	}

    return Local_u8ErrorState;

 }
uint8 ADC_u8StartSingleConversionAsync (uint8 Copy_u8Channel,uint16 * Copy_pu16Result, void(*Copy_pvNotificationFunc)(void))
{

 uint8 Local_u8ErrState=OK;

  if((Copy_pu16Result !=NULL) && (Copy_pvNotificationFunc!=NULL))

   {

	   if(ADC_u8BusyFlag==IDLE)
		{
           /*ADC IS BUSY IS NOW*/
		   ADC_u8BusyFlag=BUSY;

          /*Convert the local ADC result into global */
          ADC_pu16Result=Copy_pu16Result;
          /*Convert the local ADC Call back into global */
          ADC_pvCallBAckNotificationFunc=Copy_pvNotificationFunc;


       /*Set the input channel*/
   	   ADMUX &= CHANNEL_BIT_MASK;   /*clear the channel selection bits*/
   	   ADMUX |= Copy_u8Channel;
       /*Start Conversion*/
       SET_BIT(ADCSRA,ADCSRA_ADSC);


       /*Enable ADC Conversion complete interrupt*/
       SET_BIT(ADCSRA,ADCSRA_ADIE);
       /*ISR Source is Single Conversion Asynch*/
        ADC_u8ISRSource = SINGLE_ASYNCH;
        }
	   else
	     {

	      Local_u8ErrState=BUSY_ERR;
	     }
	}

    else
    {

    Local_u8ErrState=NULL_PTR_ERR;

    }



 return Local_u8ErrState;
}
__attribute__ ((signal)) void __vector__16(void);
void __vector__16(void)
{
    if(ADC_u8ISRSource==SINGLE_ASYNCH)
    {/* ISR COMES TO COMPLETE  THE WORK OF SINGLE ASYNCH FUNC*/
   /*Read the result*/
    #if ADC_u8Resolution ==EIGHT_BITS
	  *ADC_pu16Result= ADCH;
	#elif ADC_u8Resolution ==TEN_BITS
	  *ADC_pu16Result= ADC;
	#endif
   /*Disable conversion complete interrupt*/
   CLR_BIT(ADCSRA,ADCSRA_ADIE);
   /*ADC NOW IS IDLE*/
   ADC_u8BusyFlag=IDLE;

 /*invoke the call back notification function*/
   if(ADC_pvCallBAckNotificationFunc !=NULL)
   {
      ADC_pvCallBAckNotificationFunc();
   }
   else
   {
	   /*DO NOthing*/
   }
    }
    else if(ADC_u8ISRSource==CHAIN_ASYNCH)
	{
    	volatile static uint8 Local_u8ChannelIndex= 0u;

    	/* ISR COMES TO COMPLETE  THE WORK OF CHAIN ASYNCH FUNC*/
    	 /*Read the result*/
    	    #if ADC_u8Resolution ==EIGHT_BITS
    	      ADC_pChainData-> ResultArr[Local_u8ChannelIndex]= ADCH;
    		#elif ADC_u8Resolution ==TEN_BITS
    		  ADC_pChainData-> ResultArr[Local_u8ChannelIndex]= ADC;
    		#endif
        /*Increment the channel Counter*/
    		  Local_u8ChannelIndex++;
    		  if(Local_u8ChannelIndex == ADC_pChainData->ChainSize)
    		  {
    			  //Chain is completed
    		  }
    		  else
    		  {

    			  //chain is not finished yet ,start the next conversion
    			  /*Set the input channel*/
    			     	   ADMUX &= CHANNEL_BIT_MASK;   /*clear the channel selection bits*/
    			     	   ADMUX |= ADC_pChainData->ChannelArr[0];
    			         /*Start Conversion*/
    			         SET_BIT(ADCSRA,ADCSRA_ADSC);

    		  }
	}

}

uint8 ADC_u8StartChainConversionAsynch(const ADC_Chain_t* Copy_object)
{
	uint8 Local_u8ErrorState = OK;

	if((Copy_object != NULL) && (Copy_object->ChannelArr != NULL) && (Copy_object->ResultArr != NULL) && (Copy_object ->NotificationFunc != NULL))
	{
		if(ADC_u8BusyFlag == IDLE)
		{
			/*ADC is now busy*/
			ADC_u8BusyFlag = BUSY;

			/*Define the input chain data globally*/
			ADC_pChainData = Copy_object;

			/*Set the input channel*/
			ADMUX &= CHANNEL_BIT_MASK;		/*Clear the channel selection bits*/
			ADMUX |= ADC_pChainData->ChannelArr[0];

			/*Start conversion*/
			SET_BIT(ADCSRA, ADCSRA_ADSC);

			/*Enable ADC conversion complete interrupt*/
			SET_BIT(ADCSRA, ADCSRA_ADIE);

			/*ISR Source is Chain Conversion Asynch*/
			ADC_u8ISRSource = CHAIN_ASYNCH;
		}
		else
		{
			Local_u8ErrorState = BUSY_ERR;
		}
	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}

	return Local_u8ErrorState;
}

static void voidHandleSingleConvAsynch(void)
{
	/*ISR comes to complete the work of Start Single conversion Asynch function*/
	/*Read the result*/
#if ADC_u8Resolution	== 	EIGHT_BITS
	*ADC_pu16Result = ADCH;
#elif ADC_u8Resolution	== 	TEN_BITS
	*ADC_pu16Result = ADC;
#endif

	/*Disable conversion complete interrupt*/
	CLR_BIT(ADCSRA, ADCSRA_ADIE);

	/*ADC is now idle*/
	ADC_u8BusyFlag= IDLE;

	/*Invoke the call back notification function*/
	if(ADC_pvCallBAckNotificationFunc != NULL)
	{
		ADC_pvCallBAckNotificationFunc();
	}
	else
	{
		/*Do nothing*/
	}
}
static void voidHandleChainConvAsynch(void)
{
	volatile static uint8 Local_u8ChannelIndex = 0u;

	/*ISR comes to complete the work of Start Chain conversion Asynch function*/
#if ADC_u8Resolution	== 	EIGHT_BITS
	ADC_pChainData->ResultArr[Local_u8ChannelIndex] = ADCH;
#elif ADC_u8Resolution	== 	TEN_BITS
	ADC_pChainData->ResultArr[Local_u8ChannelIndex] = ADC;
#endif
	/*Increment the channel counter*/
	Local_u8ChannelIndex++;

	if(Local_u8ChannelIndex == ADC_pChainData->ChainSize)
	{
		/*Chain is completed*/
		/*Reset the channel Index to be prepared for next chain*/
		Local_u8ChannelIndex = 0;

		/*Disable conversion complete interrupt*/
		CLR_BIT(ADCSRA, ADCSRA_ADIE);

		/*ADC is now idle*/
		ADC_u8BusyFlag= IDLE;

		/*Invoke the call back notification function*/
		if(ADC_pChainData->NotificationFunc != NULL)
		{
			ADC_pChainData->NotificationFunc();
		}
		else
		{
			/*Do nothing*/
		}
	}
	else
	{
		/*Chain is not finished yet, start the next conversion*/
		/*Set the input channel*/
		ADMUX &= CHANNEL_BIT_MASK;		/*Clear the channel selection bits*/
		ADMUX |= ADC_pChainData->ChannelArr[Local_u8ChannelIndex];

		/*Start conversion*/
		SET_BIT(ADCSRA, ADCSRA_ADSC);
	}
}


/*ADC conversion complete ISR*/
__attribute__((signal)) void __vector_16 (void);
void __vector_16 (void)
{
	if(ADC_u8ISRSource == SINGLE_ASYNCH)
	{
		voidHandleSingleConvAsynch();
	}

	else if(ADC_u8ISRSource == CHAIN_ASYNCH)
	{
		voidHandleChainConvAsynch();
	}

}
