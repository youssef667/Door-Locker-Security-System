/*
 * TIMER0.c
 *
 *  Created on: Nov 1, 2021
 *      Author: youssef
 */

#include"TIMER0.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Common_Macros.h"

#define NULL_PTR ((void*)0)

static volatile void (*g_callBackPtr)(void) = NULL_PTR;

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

void Set_callBack(void(*a_ptr)(void)){
	g_callBackPtr=a_ptr;
}


void Timer0_init(const Timer0_ConfigType * Config_Ptr){

	TCNT0= Config_Ptr->initialValue;
	SET_BIT(TCCR0,FOC0); /* for non-PWM */

	//configue clock
	TCCR0 = 0;
	TCCR0 |= (Config_Ptr->clock);

	if ((Config_Ptr->mode==NORMAL_MODE)){
			SET_BIT(TIMSK,TOIE0);  /*Interrupt for overflow mode  */
			SET_BIT(TIFR,TOV0); /* enable overflow flag */

		}

	else if ((Config_Ptr->mode==CTC)) {
			SET_BIT(TIMSK,OCIE0);
			SET_BIT(TIFR,OCF0); /* enable CTC flag */
			SET_BIT(TCCR0,WGM01);
			//configure OCR
			OCR0=Config_Ptr->compareValue;

		}

}


void timer0_DeInit(){
	TCCR0 = 0;
	TCNT0 = 0;
	OCR0 = 0;
	CLEAR_BIT(TIMSK,OCIE0);
	CLEAR_BIT(TIMSK,TOIE0);

}
