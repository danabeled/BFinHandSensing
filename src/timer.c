/**
 *@file timer.h
 *
 *@brief
 * the timer is using the TIMER2 
 *
 *
 *@author    Zhen Jiang 
 *@date      03/21/2014
 *
 * LastChange:
 *
 ******************************************************************************/
#include <stdio.h>
#include "timer.h"
#include "constant.h"
#include "cdefBF52x_base.h"

const int TIMER_POSITION = 2;
long startCountValue = 0;

int timer_init(){
    //disable the timer
	*pPORTG_FER |= PG5;
	*pTIMER_STATUS = TIMIL1 | TOVF_ERR1 | TRUN1;
    timer_stop();
    *pTIMER2_PERIOD = 0xFFFFFFF; //Set to be arbitrarily large
    *pTIMER2_CONFIG = PWM_OUT| PULSE_HI | PERIOD_CNT;
    *pTIMER2_WIDTH = 0x249F0;
    return SUCCESSFUL;
}
int timer_start(){
    //start timer counting
    *pTIMER_DISABLE &= ~(1 << TIMER_POSITION);
	*pTIMER_ENABLE |= 1 << TIMER_POSITION;
	startCountValue = *pTIMER2_COUNTER;
    return SUCCESSFUL;
}
int timer_stop(){
    //stop timer counting
	*pTIMER_DISABLE |= 1 << TIMER_POSITION;
    *pTIMER_ENABLE &= ~(1 << TIMER_POSITION);
    return SUCCESSFUL;
}
int timer_getValue(int* value){
    //get timer value
	if(*pTIMER2_COUNTER < startCountValue){
		*value = (*pTIMER2_PERIOD - startCountValue) + *pTIMER2_COUNTER;
	}else{
		*value = *pTIMER2_COUNTER - startCountValue;
	}
    return SUCCESSFUL;
}


