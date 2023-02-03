/*
 * main.h
 *
 *  Created on: Nov 4, 2021
 *      Author: youssef
 */

#ifndef MAIN_H_
#define MAIN_H_
#include "std_types.h"
/*********************************************************************************/
#define password_lengh                          5
#define motor_worktime                          15
#define door_open_period			    	    3
#define number_of_time_to_enter_password_wrong 	3
#define buzzer_delay                            30

/* following definitions used to communicate the controller that handle the human machine interface */

#define PASSWORD_MATCHED		1
#define PASSWORD_MISMATCHED		0
#define READY_TO_SEND			0x15
#define READY_TO_RECEIVE		0x16
#define CHANGE_PASSWORD      	0x18
#define WRONG_PASSWORD			0x30
#define CHANGING_PASSWORD		0X31
#define UNLOCKING_DOOR			0x25
#define TWI_CONTROL_ADRESS   	0x1
#define EEPROM_ADDREESS		    0x00
/********************************************************/
/* GLOBAL VARIABLES */
/********************************************************/
uint8 PassWord [password_lengh];

uint8 PassWord_recieved [password_lengh];

uint8 wrong_password_counter =0;

uint8 seconds =0;

uint16 micro_seconds =0;

/********************************************************/
              /* functions */
/********************************************************/
/* TO COMPARE STORED PASS WITH TH RECIVED ONE */
uint8 compare_passwords(uint8 a_password1[password_lengh],uint8 a_password2[password_lengh]);
/* THIS FUNCTION CALLED BY TIMER AND INDICATES THE NUMBER OF SECONDS*/
void call_back_fun(void);
/* A function to retrieve the stored password from EEPROM*/
void updateStoredPassword(void);
/* A function that use the dcmotor driver functions to control the motor  /
void DoorOpeningTask(void);
/*Description: a function to initialize the password in first-run OR to change the password*/
void password_init(void);
/* A function to receive the password via UART by looping on receiveByte function*/
void receivePasswordViaUART(uint8 * passwordArray);
/* FUNCTION TO STORE PASSWORD IN THE EXTERNAL MEMORY*/
void storePassword(void);


#endif /* MAIN_H_ */
