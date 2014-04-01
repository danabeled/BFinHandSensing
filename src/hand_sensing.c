#include <stdio.h>
#include "startup.h"
#include "constant.h"
#include "hand_sensing.h"
#include "charger.h"
#include "cdefBF52x_base.h"
#include "point.h"
#include "queueHandler.h"

void handSensing(){
	if(fpga_outputToPortFEnable(TLLXBV1_INPUT_PINS)<0){
		printf("failed to enable FPGA Output pins\r\n");
		exit(-1);
	}
//	queueHandler_init();
//
//	point_t point1;
//	point1.x_pos = 100;
//	point1.y_pos = 100;
//	point1.z_pos = 100;
//	point_print(&point1);
//	queueHandler_pushPoint(&point1);
//	queueHandler_draw();
//
//	point_t point2;
//	point2.x_pos = 100;
//	point2.y_pos = 200;
//	point2.z_pos = 100;
//	queueHandler_pushPoint(&point2);
//	queueHandler_draw();
//
//
//	point_t point3;
//	point3.x_pos = 200;
//	point3.y_pos = 200;
//	point3.z_pos = 200;
//	queueHandler_pushPoint(&point3);
//	queueHandler_draw();
//
//	coreTimer_delay(0xFFFFFFFF);
//
//	queueHandler_clear();
//	coreTimer_delay(0xFFFFFFFF);
//
//	point_t point4;
//	point4.x_pos = 200;
//	point4.y_pos = 100;
//	point4.z_pos = 100;
//	point_print(&point4);
//	queueHandler_pushPoint(&point4);
//	queueHandler_draw();
//
//	point_t point5;
//	point5.x_pos = 100;
//	point5.y_pos = 100;
//	point5.z_pos = 200;
//	queueHandler_pushPoint(&point5);
//	queueHandler_draw();
//
//	coreTimer_delay(0xFFFFFFFF);

    charger_t charger;
    charger_init(&charger);
    //charger_debug_enable();

    while(1){
    	if(ERROR == charger_run(&charger)){
    		charger.newDataFlag = 0;
    		continue;
    	}
        printf("%d %d %d\r\n", charger.xTime, charger.yTime, charger.zTime);
        charger.newDataFlag = 0;
        //queueHandler_draw();
    }
}
