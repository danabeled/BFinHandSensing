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

int timer_init(){
    //disable the timer 2
    *pTIMER_DISABLE = 1 << 2;
    return SUCCESSFUL;
}
int timer_start(){
    //start timer counting
    *pTIMER_ENABLE = 1 << 2;
    return SUCCESSFUL;
}
int timer_stop(){
    //stop timer counting
    *pTIMER_DISABLE = 1 << 2;
    return SUCCESSFUL;
}
int timer_getValue(int* value){
    //get timer value
    *value =  *pTIMER2_COUNTER;
    return SUCCESSFUL;
}


