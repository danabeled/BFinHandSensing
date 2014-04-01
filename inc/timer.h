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
#ifndef _TIMER_H_
#define _TIMER_H_

/**
  * This is the initialization for the timer
  * Paramters:
  *
  * @return 0 on success, otherwise ERROR
  */
int timer_init();
/**
  * This is function starts the timer counting
  * Paramters:
  *
  * @return 0 on success, otherwise ERROR
  */
int timer_start();
/**
  * This is function stops the timer counting
  * Paramters:
  *
  * @return 0 on success, otherwise ERROR
  */
int timer_stop();
/**
  * This is function gets the timer counting value
  * Paramters:
  *
  * @return 0 on success, otherwise ERROR
  */
int timer_getValue(long* value);

#endif
