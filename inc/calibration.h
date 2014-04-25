/**************************************************************************************
	@file: calibration.h
	@brief: Calibration finds the baseline and range value for each plate.
	@author: Zhen Jiang
	@created: April 1, 2014
**************************************************************************************/

#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "charger.h"

/*********************************Data Type Definitions****************************/


/********************************* Method Prototypes *******************************/
/**/
/**
  * calibrate function find baseline and range value for each plate
  * Paramters: pThis, the charger sensor needed to calibrate
  *
  * @return void
  */
void calibrate(charger_t* pThis);
/**
  * calibrate_baseline function find baseline for each plate
  * Paramters: pThis, the charger sensor need to re-calculate the baseline
  *
  * @return void
  */
void calibrate_baseline(charger_t* pThis);

#endif

