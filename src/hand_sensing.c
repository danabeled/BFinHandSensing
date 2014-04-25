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

struct timeval time_start, time_stop;
struct timeval time_charge_begin, time_charge_end;
int diff_x = 0;
int diff_y = 0;
int diff_z = 0;

int negative_count = 0;
#define NEGATIVE_MAX 20

int pos_count = 0;

double ratio = 1;

charger_t charger;
point_t point1;

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

		if((diff_x > RADIUS && diff_x <= charger.range_x )
				&& (diff_y > RADIUS && diff_y <= charger.range_y)){
			point1.x_pos = diff_x;
			point1.y_pos = diff_y;
			point1.z_pos = diff_z;
			queueHandler_pushPoint(&point1);
			queueHandler_draw();
			//gettimeofday(&time_stop, NULL);
			//printf("\r\ntime diff: %d\r\n", (int)time_stop.tv_usec - (int)time_start.tv_usec);
			if(ratio > 0){
				printf("In range");
			}
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
