/*
 * buzzer.c
 *
 *  Created on: Nov 2, 2021
 *      Author: youssef
 */

#include "buzzer.h"
#include "gpio.h"
void buzzer_init (void){
	GPIO_setupPinDirection(buzzer_port, buzzer_pin, PIN_OUTPUT);
	 buzzer_off(); // buzzer off at the beginning
}
void buzzer_on (void){
	GPIO_writePin(buzzer_port, buzzer_pin, LOGIC_HIGH);
}
void buzzer_off (void){
	GPIO_writePin(buzzer_port, buzzer_pin, LOGIC_LOW);
}
