/*
 * buzzer.h
 *
 *  Created on: Nov 2, 2021
 *      Author: youssef
 */

#ifndef BUZZER_H_
#define BUZZER_H_
#include "gpio.h"

#define buzzer_port   PORTC_ID
#define buzzer_pin    PIN3_ID


void buzzer_init (void);
/* function to set the buzzer pin*/
void buzzer_on (void);
/*function to clear the buzzer pin*/
void buzzer_off (void);
#endif /* BUZZER_H_ */
