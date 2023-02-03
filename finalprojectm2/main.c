/*
 * main.c
 *
 *  Created on: Nov 4, 2021
 *      Author: youssef
 */


#include "main.h"
#include "buzzer.h"
#include "TIMER0.h"
#include "dc_motor.h"
#include "external_eeprom.h"
#include "twi.h"
#include <avr/io.h>
#include <avr/delay.h>
#include "uart.h"


uint8 compare_passwords(uint8 a_password1[password_lengh],uint8 a_password2[password_lengh]){

	updateStoredPassword();
		uint8 i;
		for (i = 0; i < password_lengh; i++) {
			if (a_password1[i] != a_password2[i]) {
				return PASSWORD_MISMATCHED;
			}
		}
		return PASSWORD_MATCHED;

}


void updateStoredPassword(void){

	uint8 i;
		for (i=0;i<password_lengh;i++){
			EEPROM_readByte(EEPROM_ADDREESS	+i, PassWord+i);
		}
}

void DoorOpeningTask(void){
	/* run the DC motor clockwise for 15 seconds */
	seconds = 0;
	DcMotor_Rotate(DC_MOTOR_CW);
	while (seconds < motor_worktime);

	/* let the door be open for 3 seconds */
	seconds = 0;
	DcMotor_Rotate(DC_MOTOR_STOP);
	while (seconds < door_open_period);

	/* hold the system for 15 seconds & display to user that door is locking */
	seconds = 0;
	DcMotor_Rotate(DC_MOTOR_ACW);
	while (seconds < motor_worktime);

	DcMotor_Rotate(DC_MOTOR_STOP);
}

void call_back_fun(void){
	micro_seconds++ ;
	if(micro_seconds==31)/* the seconds increase after 31 overflow */
	{
		seconds++;
		micro_seconds=0;
      }
}

void password_init(void){
	/* do not return from this function till Control ECU sends a PASSWORD_MATCH signal */
		uint8 confirmationPassword[password_lengh ];
		uint8 check=0;
		while(!check){
			while (UART_recieveByte() != READY_TO_SEND); /* wait till HMI gets ready */
			UART_sendByte(READY_TO_RECEIVE); /* inform HMI that Control ECU ready to receive the password */
			receivePasswordViaUART(PassWord_recieved);

			while (UART_recieveByte() != READY_TO_SEND);
			UART_sendByte(READY_TO_RECEIVE); /* inform HMI to send the confirmation password */
			receivePasswordViaUART(confirmationPassword);

			if (compare_passwords(PassWord_recieved, confirmationPassword) == PASSWORD_MATCHED){
				UART_sendByte(READY_TO_SEND);
				UART_sendByte(PASSWORD_MATCHED);
				storePassword();
				check=1;
			}else{
				UART_sendByte(READY_TO_SEND);
				UART_sendByte(PASSWORD_MISMATCHED);
		}
	}
}

void receivePasswordViaUART(uint8 * passwordArray){
	uint8 cnt;
		for (cnt=0;cnt<password_lengh;cnt++){
			*(passwordArray+cnt) = UART_recieveByte();
			_delay_ms(100);
		}
}
void storePassword(void){
	uint8 i;
	for (i = 0; i < password_lengh; i++) {
		EEPROM_writeByte(EEPROM_ADDREESS + i, PassWord_recieved[i]);
		_delay_ms(100);
	}
}
int main(void){

	SREG |=(1<<SREG_I); // enable I-BIT
		UART_configType usart_con = {9600,NO_Parity, One_Stop_Bit, Data_8_Bits};
		UART_init(&usart_con);
		Set_callBack(call_back_fun);

		Timer0_ConfigType timer_con = { NORMAL_MODE, F_CPU_1024,0,0 };
		Timer0_init(&timer_con );

		TWI_Configurations config = {0x02, TWI_CONTROL_ADRESS };
		TWI_init(&config);
		DcMotor_Init();
		buzzer_init ();
		password_init();
		uint8 receivedByte=0;
	while(1){
		if (UART_recieveByte() == READY_TO_SEND){
		receivePasswordViaUART(PassWord_recieved);
		receivedByte = UART_recieveByte();

		if ( receivedByte == '+'){
		if (compare_passwords(PassWord, PassWord_recieved) == PASSWORD_MATCHED){
		UART_sendByte(UNLOCKING_DOOR); /* inform HMI ECU to display that door is unlocking */
		DoorOpeningTask(); /* start opening door process/task */
		}else{
		UART_sendByte(WRONG_PASSWORD);
		/* count number of wrong attempts, and turn on a buzzer of it exceeds the limit */
		wrong_password_counter++;
	    if (wrong_password_counter == number_of_time_to_enter_password_wrong){
	    buzzer_on();
	    seconds=0;
	    while(seconds < buzzer_delay); /* turn on alarm for a certain period */
	    buzzer_off();
	    wrong_password_counter=0; /* reset the counter */
		}
	}


	   } else if (receivedByte == CHANGING_PASSWORD) {
		if (compare_passwords(PassWord, PassWord_recieved) == PASSWORD_MATCHED) {
		UART_sendByte(CHANGING_PASSWORD); /* inform HMI to process changing password */
		password_init();
		}else{
		UART_sendByte(WRONG_PASSWORD);
						}
					}
				}
	}
}
