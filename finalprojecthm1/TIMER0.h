/*
 * TIMER0.h
 *
 *  Created on: Nov 1, 2021
 *      Author: youssef
 */

#ifndef TIMER0_H_
#define TIMER0_H_

#include "std_types.h"
#include "common_macros.h"

typedef enum{
	NORMAL_MODE, CTC
}TIMER0_MODE;

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer0_Clock;

typedef struct{
	TIMER0_MODE mode;
	Timer0_Clock clock;
	uint16 compareValue;
	uint16 initialValue;
}Timer0_ConfigType;

//prototypes

/*A function that initialize a timer with dynamic configurations*/

void Timer0_init(const Timer0_ConfigType * Config_Ptr);

/*this function responsible for select the function that ISR want to call back*/

void Set_callBack(void(*a_ptr)(void));
/*
 the function disables: the clock, overflow & CTC interrupts
  *the function clears: initial value, compare value & the call-back function
 * */
void timer0_DeInit(void);

#endif /* TIMER0_H_ */
