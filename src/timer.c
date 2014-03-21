#include "timer.h"
#include <stdio.h>

int initTimer(){
    //disable the timer 2
    *pTIMER_DISABLE = 1 << 2;
}
int startTimer(){
    *pTIMER_ENABLE = 1 << 2;
}
int stopTimer(){
    *pTIMER_DISABLE = 1 << 2;
}
int getTimerValue(){
    return *pTIMER2_COUNTER;
}


