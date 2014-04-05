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
#include <sys/time.h>
#include <cycle_count.h> // make sure to include these headers
#include <tll6527_core_timer.h>
#include "constant.h"
#include "cdefBF52x_base.h"

#define DO_CYCLE_COUNTS

struct timeval time_begin, time_end;

int timer_init(){
	RTC_waitForInit();
    return SUCCESSFUL;
}
int timer_start(){
	gettimeofday(&time_begin, NULL);
    return SUCCESSFUL;
}
int timer_stop(){
	return SUCCESSFUL;
}
int timer_getValue(long* value){
    //get timer value
	gettimeofday(&time_end, NULL);
	*value = (int)time_end.tv_usec - (int)time_begin.tv_usec;
    return SUCCESSFUL;
}


