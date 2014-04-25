/**************************************************************************************
	@file: hand_sensing.c
	@brief: main application class, contains the while(1)
	@author: Zhen Jiang
	@created: March 23, 2014
	@updated: Zhen Jiang 04/01/2014
**************************************************************************************/
#include <stdio.h>
#include "startup.h"
#include "constant.h"
#include "hand_sensing.h"
#include "charger.h"
#include "cdefBF52x_base.h"
#include "point.h"
#include "queueHandler.h"
#include "ssvep.h"
#include "calibration.h"
#include <sys/time.h>

//result measurement variables
struct timeval time_start, time_stop;
struct timeval time_charge_begin, time_charge_end;

//value after subtracted from baseline
int diff_x = 0;
int diff_y = 0;
int diff_z = 0;

//recalibration counter
int negative_count = 0;
#define NEGATIVE_MAX 20
int pos_count = 0;

//sensor cube
charger_t charger;

//circle on the LCD
point_t point1;

//
void handSensing(){

	queueHandler_init();
    charger_init(&charger);
    queueHanlder_drawTextAtCenter("Calibration...");

    //calibration
    calibrate(&charger);
    setXRange(charger.range_x);
    setYRange(charger.range_y);
    setZRange(charger.range_z);

    charger.x_state = NOT_READY;
    charger.y_state = NOT_READY;
    charger.z_state = NOT_READY;

	queueHanlder_drawTextAtCenter("System Ready...");

    while(1){

    	//recalibration interrupt
    	if(charger.y_state == READY){
    		charger.x_state = NOT_READY;
			charger.y_state = NOT_READY;
			charger.z_state = NOT_READY;
    		charger.calibration_state = BASELINE;
    		calibrate(&charger);
			setXRange(charger.range_x);
			setYRange(charger.range_y);
			setZRange(charger.range_z);

			charger.x_state = NOT_READY;
			charger.y_state = NOT_READY;
			charger.z_state = NOT_READY;
			continue;
    	}

    	//gettimeofday(&time_start, NULL);

    	if(ERROR == charger_run(&charger)){
    		charger.newDataFlag = 0;
    		continue;
    	}

		diff_x = charger.xTime - charger.baseline_x;
		diff_y = charger.yTime - charger.baseline_y;
		diff_z = charger.zTime - charger.baseline_z;

		//recalibration detection
		if(diff_x < -50 || diff_y < -50 || diff_z < -50){
			negative_count++;
			if(negative_count == NEGATIVE_MAX){
				negative_count = 0;
				calibrate_baseline(&charger);
				continue;
			}
		}else{
			negative_count = 0;
		}

		//valid point
		if((diff_x > RADIUS && diff_x <= charger.range_x )
				&& (diff_y > RADIUS && diff_y <= charger.range_y)){
			point1.x_pos = diff_x;
			point1.y_pos = diff_y;
			point1.z_pos = diff_z;
			queueHandler_pushPoint(&point1);
			queueHandler_draw();
			//gettimeofday(&time_stop, NULL);
			//printf("\r\ntime diff: %d\r\n", (int)time_stop.tv_usec - (int)time_start.tv_usec);
			printf("In range");

		}else{
			printf("Out range.");
		}

		printf("Time: %f %f %f, Diff: %f %f %f, Range: %d %d %d\r\n",
			charger.xTime, charger.yTime, charger.zTime,
			diff_x, diff_y, diff_z,
			charger.range_x, charger.range_y, charger.range_z);

        charger.newDataFlag = 0;
    }
}
