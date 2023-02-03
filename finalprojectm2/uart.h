 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_


#include "std_types.h"

#include <avr/interrupt.h>
#include <util/delay.h>
/*******************************************************************************
 *                      Type definitions                                    *
 *******************************************************************************/
typedef enum{
	NO_Parity, Reserved, Even_Parity, Odd_Parity
}UART_ParityType;

typedef enum{
	One_Stop_Bit, Two_Stop_Bit
}UART_StopBitType;

typedef enum{
	Data_5_Bits, Data_6_Bits, Data_7_Bits, Data_8_Bits, Data_9_Bits = 7
}UART_DataBitType;

typedef struct{
	uint32 baud_rate;
	UART_ParityType parityConfig;
	UART_StopBitType stopBitConfig;
	UART_DataBitType dataType;
}UART_configType;



/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const UART_configType * configPtr);

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
