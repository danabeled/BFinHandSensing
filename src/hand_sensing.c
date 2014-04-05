#include <stdio.h>
#include "startup.h"
#include "constant.h"
#include "hand_sensing.h"
#include "charger.h"
#include "cdefBF52x_base.h"
#include "point.h"
#include "queueHandler.h"
#include "ssvep.h"

void handSensing(){

	queueHandler_init();

	point_t point1;
	point1.x_pos = 100;
	point1.y_pos = 100;
	point1.z_pos = 100;
	queueHandler_pushPoint(&point1);
	queueHandler_draw();

	queueHandler_display();
	coreTimer_delay(0xFFFFFFFF);

	gpio_init();
    charger_t charger;
    charger_init(&charger);

    while(1){
    	if(ERROR == charger_run(&charger)){
    		charger.newDataFlag = 0;
    		continue;
    	}
        printf("%d %d %d\r\n", charger.xTime, charger.yTime, charger.zTime);
    }
}
