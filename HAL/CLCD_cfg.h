#ifndef CLCD_CFG_H
#define CLCD_CFG_H

/*configure the lcd bit mode, options are :		1- FOUR_BIT
 * 												2- EIGHT_BIT
 * 	*/
#define CLCD_u8BIT_MODE			EIGHT_BIT

// configure if the clcd RW pin is connected to gnd or DIO pin

#define CLCD_u8RW_CONN_STS    GND_CONNECTED


#define CLCD_u8CTRL_PORT		DIO_u8PORTA

#define CLCD_u8RS_PIN			DIO_u8PIN1

#if CLCD_u8RW_CONN_STS==DIO_CONNECTED
#define CLCD_u8RW_PIN			DIO_u8PIN2
#endif

#define CLCD_u8E_PIN			DIO_u8PIN3

#define CLCD_u8DATA_PORT		DIO_u8PORTB///

#if CLCD_u8BIT_MODE == FOUR_BIT
#define CLCD_u8D4_PIN			DIO_u8PIN4
#define CLCD_u8D5_PIN			DIO_u8PIN5
#define CLCD_u8D6_PIN			DIO_u8PIN6
#define CLCD_u8D7_PIN			DIO_u8PIN7
#endif

#endif
