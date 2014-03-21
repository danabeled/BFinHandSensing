#ifndef _TIMER_H_
#define _TIMER_H_

#ifndef PLATFORM
#error "The code is specific to TLL6527M. Please check the project properties"
#endif

int initTimer();
int startTimer();
int stopTimer();
int getTimerValue();

#endif
