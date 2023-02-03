/*
 * main.h
 *
 *  Created on: Nov 4, 2021
 *      Author: youssef
 */

#ifndef MAIN_H_
#define MAIN_H_
#include "std_types.h"
/********************************************************/
/* Definitions & Configurations */
/********************************************************/
#define password_lengh        5
#define keypad_delay          3000
#define motor_worktime        15   //protous simulation has problem in real time operating but the motor work time is equal 15 sec
#define lcd_display_delay     5000
/*the following definitions are made to communicate with the controller that handle the password storing and comparing  */
#define PASSWORD_MATCHED		1
#define PASSWORD_MISMATCHED		0
#define READY_TO_SEND			0x15
#define READY_TO_RECEIVE		0x16
#define CHANGE_PASSWORD      	0x18
#define UNLOCKING_DOOR			0x25
#define WRONG_PASSWORD			0x30
#define CHANGING_PASSWORD		0X31

/********************************************************/
/* GLOBAL VARIABLES */
/********************************************************/
uint8 PassWord[password_lengh];

uint8 password_match_status = 0;
uint16 micro_seconds =0;
uint8 seconds =0;
/********************************************************/
              /* functions */
/********************************************************/

/*called by timer and this function resbosible for increase seconds */
void call_back_fun(void);
/*this function to get the password from user and store it in arry*/
void get_password (uint8 *password_array);
/*this function to send the password via UART by looping on sendByte function*/
void usart_send_password (uint8 *password_array);
/*this function that displays on LCD that door is opening or closing for a certain period of time*/
void door_unlock (void);
/* this function to initialize the password in first-run OR to change the password*/
void password_init (void);
/*this function to display application options on LCD*/
void app_options(void);
#endif /* MAIN_H_ */
