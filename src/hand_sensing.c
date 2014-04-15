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

int diff_x = 0;
int diff_y = 0;
int diff_z = 0;


void handSensing(){

	queueHandler_init();

    charger_t charger;
    charger_init(&charger);

	point_t point1;
	point1.x_pos = 100;
	point1.y_pos = 100;
	point1.z_pos = 100;
	queueHandler_pushPoint(&point1);
	queueHandler_draw();
	queueHandler_display();

    //calibration
    calibrate(&charger);
    setXRange(charger.range_x);
    setYRange(charger.range_y);
    setZRange(charger.range_z);

	printf("\r\n\n\n System Ready...\r\n\n\n");
    while(1){
    	if(ERROR == charger_run(&charger)){
    		charger.newDataFlag = 0;
    		continue;
    	}

		diff_x = charger.xTime - charger.baseline_x;
		diff_y = charger.yTime - charger.baseline_y;
		diff_z = charger.zTime - charger.baseline_z;

		printf("diff: %d \r\n", diff_x);
		if((diff_x > RADIUS ) || (diff_y > RADIUS)){
			point1.x_pos = diff_x;
			point1.y_pos = diff_y;
			point1.z_pos = diff_z;
			queueHandler_pushPoint(&point1);
			queueHandler_draw();
		}else{
			printf("Out range\r\n");
		}

		printf("hand: %lu %lu %lu\r\n", charger.xTime, charger.yTime, charger.zTime);
        charger.newDataFlag = 0;
    }
}
