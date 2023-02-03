/*
 * main.c
 *
 *  Created on: Nov 4, 2021
 *      Author: youssef
 */

#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "std_types.h"
#include "TIMER0.h"
#include "main.h"
#include <avr/io.h>
#include <avr/delay.h>

void call_back_fun(void)
{
	micro_seconds++ ;
	if(micro_seconds==31)/* the seconds increase after 31 overflow */
	{
		seconds++;
		micro_seconds=0;
      }
	}


void get_password (uint8 *password_array){
	LCD_moveCursor(1, 0);
	uint8 key;
	uint8 i =0;
    while(i !=password_lengh)
    {
    	key =  KEYPAD_getPressedKey();
    	if (key >=0 && key <=9)
    	{
    		 LCD_displayCharacter("*");
    		 *(password_array +i)= key;
    		 i++;
    	}
    	_delay_ms(keypad_delay);
    }
    key =0;
    /* stay till user pushes on/0f button */
    while (KEYPAD_getPressedKey() !=13);
}


void usart_send_password (uint8 *password_array){

	uint8 i;
		for (i=0;i<password_lengh;i++){
			UART_sendByte(PassWord[i]);
			_delay_ms(100);
		}
}


void door_unlock (void){
	seconds = 0;
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Door is unlocking");
		while (seconds < motor_worktime);

		/* let the door be open for 3 seconds */
		seconds = 0;
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Door is now open");
		while (seconds < motor_worktime);

		/* hold the system for 15 seconds & display to user that door is locking */
		seconds = 0;
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Door is locking");
		while (seconds < motor_worktime);
}


void password_init (void){

	while(password_match_status == PASSWORD_MISMATCHED){
			LCD_clearScreen();
			LCD_displayString("Enter password");
			LCD_moveCursor(1, 0);
			get_password(PassWord); /* get the password from user */
			UART_sendByte(READY_TO_SEND);
			while (UART_recieveByte() != READY_TO_RECEIVE); /* wait for a response */
			usart_send_password(PassWord);

			/* get confirm password from user */
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Re-enter password");
			LCD_moveCursor(1, 0);
			get_password(PassWord);
			UART_sendByte(READY_TO_SEND);
			while (UART_recieveByte() != READY_TO_RECEIVE);
			usart_send_password(PassWord);

			/* wait for a response from Control ECU about passwords matching */
			while (UART_recieveByte() != READY_TO_SEND);
			password_match_status = UART_recieveByte();

			if (password_match_status == PASSWORD_MISMATCHED){
				LCD_clearScreen();
				LCD_displayString("Passwords mismatch");
				_delay_ms(lcd_display_delay);
			}
		}
	       password_match_status = PASSWORD_MISMATCHED; /* reset password match to MISMATCH for later usage */
}

void app_options(void){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+: open door");
	LCD_displayStringRowColumn(1, 0, "-: change password");
}

int main(void)
{
	SREG |=(1<<SREG_I); // enable I-BIT

	UART_configType usart_con = {9600,NO_Parity, One_Stop_Bit, Data_8_Bits};

	UART_init(&usart_con);

	Set_callBack(call_back_fun);

	Timer0_ConfigType timer_con = { NORMAL_MODE, F_CPU_1024,0,0 };

	Timer0_init(&timer_con );

	LCD_init();

	password_init ();
	app_options();
	/* static variables*/
	uint8 receivedByte=0,key=0;
	while(1){

		key = KEYPAD_getPressedKey();
		if (key == '+') {
		LCD_clearScreen();
		LCD_displayString("Enter password");
		get_password (PassWord);
		UART_sendByte(READY_TO_SEND); /* inform Control ECU to start sending */
		usart_send_password (PassWord);
		/* inform Control ECU the option that user chose */
		UART_sendByte('+');

		/* Control ECU responses [either the password is correct or wrong] */
		receivedByte = UART_recieveByte();
		if (receivedByte == UNLOCKING_DOOR) {
			door_unlock (); /* start displaying door status on LCD */

		} else if (receivedByte == WRONG_PASSWORD) {
		LCD_clearScreen();
		LCD_displayString("Wrong password");
    	_delay_ms(lcd_display_delay );
					}
		app_options(); /* system back to idle & display main options */


		} else if (key == '-') {
		LCD_clearScreen();
		LCD_displayString("Enter current password");
		get_password (PassWord);
		UART_sendByte(READY_TO_SEND); /* inform Control ECU to start sending */
		usart_send_password (PassWord);

					/* inform Control ECU the option that user chose */
		UART_sendByte(CHANGE_PASSWORD );

		receivedByte = UART_recieveByte();
		if (receivedByte == CHANGING_PASSWORD) {
			password_init();
		LCD_clearScreen();
		} else if (receivedByte == WRONG_PASSWORD) {
		LCD_clearScreen();
		LCD_displayString("Wrong password");
	   _delay_ms(lcd_display_delay );
					}
		app_options();
		}
	}
}
