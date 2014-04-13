#include <stdio.h>
#include "startup.h"
#include "constant.h"
#include "hand_sensing.h"
#include "charger.h"
#include "cdefBF52x_base.h"
#include "point.h"
#include "queueHandler.h"
#include "ssvep.h"

#define CALIBRATION	10
#define FUNCTION	11

#define MAX_COUNT 20

short unsigned state = CALIBRATION;
double average_x;
double average_y;
double average_z;
short unsigned count = 0;
double std_x;
double std_y;
double std_z;
long sum_x = 0;
long sum_y = 0;
long sum_z = 0;

int dataset_x[MAX_COUNT];
int dataset_y[MAX_COUNT];
int dataset_z[MAX_COUNT];

double sqrt(const double x){
	union{
		int i;
		double x;
	} u;
	u.x = x;
	u.i = (1<<29) + (u.i >> 1) - (1<<22);

	u.x =       u.x + x/u.x;
	u.x = 0.25f*u.x + x/u.x;

	return u.x;
}
double calculateStd(int*dataset, int datalen, double average){
	if(datalen == 0) return 0;
	if(datalen == 1) return average;
	int i;
	int tmpSum = 0;
	for(i = 0; i < datalen; i++){
		tmpSum += (dataset[i] - average)*(dataset[i] - average);
	}
	double tmpVariance = ((double)tmpSum)/datalen;
	return sqrt(tmpVariance);
}

void handSensing(){
	queueHandler_init();
	point_t point1;
	point1.x_pos = 100;
	point1.y_pos = 100;
	point1.z_pos = 100;
	queueHandler_pushPoint(&point1);
	queueHandler_draw();
	queueHandler_display();

    charger_t charger;
    charger_init(&charger);
    setXRange(4000);

    printf("test sqrt: %f\r\n", sqrt(49));

    while(1){
    	if(ERROR == charger_run(&charger)){
    		charger.newDataFlag = 0;
    		continue;
    	}

    	if(charger.xTime == -1){
    		printf("Test Done\r\n");
    		return;
    	}
    	if(state == CALIBRATION){
    		sum_x += charger.xTime;
    		sum_y += charger.yTime;
    		sum_z += charger.zTime;

    		dataset_x[count] = charger.xTime;
    		dataset_y[count] = charger.yTime;
    		dataset_z[count] = charger.zTime;

    		count++;
    	}else{
    		long diff_x = charger.xTime - average_x;
    		long diff_y = charger.yTime - average_y;
    		long diff_z = charger.zTime - average_z;

    		printf("diff: %lu \r\n", diff_x);
    		if((diff_x > 0 && diff_x > std_x) ||
    				(diff_y > 0 && diff_y > std_y) ||
    				(diff_z > 0 && diff_z > std_z)){
    			point1.x_pos = diff_x;
    			//point1.y_pos = diff_y;
    			//point1.z_pos = diff_z;
    			point1.y_pos = 100;
				point1.z_pos = 100;
    			queueHandler_pushPoint(&point1);
				queueHandler_draw();
    		}
    	}

    	if(count == MAX_COUNT){
    		average_x = sum_x / count;
    		average_y = sum_y / count;
    		average_z = sum_z / count;

    		std_x = calculateStd(dataset_x, count, average_x);
    		std_y = calculateStd(dataset_y, count, average_y);
    		std_z = calculateStd(dataset_z, count, average_z);
    		printf("average: %f %f %f\r\n", average_x, average_y, average_z);
    		printf("std: %f %f %f\r\n", std_x, std_y, std_z);
    		state = FUNCTION;
    		count++;
    	}
        charger.newDataFlag = 0;
    }
}
