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
int diff_x = 0;
int diff_y = 0;
int diff_z = 0;

int dataset_x[MAX_COUNT];
int dataset_y[MAX_COUNT];
int dataset_z[MAX_COUNT];

#define SQRT_MAGIC_F 0x5f3759df
double sqrt(const double x){
	const float xhalf = 0.5f*x;

	  union // get bits for floating value
	  {
	    float x;
	    int i;
	  } u;
	  u.x = x;
	  u.i = SQRT_MAGIC_F - (u.i >> 1);  // gives initial guess y0
	  return x*u.x*(1.5f - xhalf*u.x*u.x);// Newton step, repeating increases accuracy
}
double calculateStd(int*dataset, int datalen, double average){
	if(datalen == 0) return 0;
	if(datalen == 1) return dataset[0];
	int i;
	int tmpSum = 0;
	for(i = 0; i < datalen; i++){
		tmpSum += (dataset[i] - average)*(dataset[i] - average);
		printf("%lu\r\n", dataset[i]);
	}
	double tmpVariance = ((double)tmpSum)/datalen;
	double tmpStd = sqrt(tmpVariance);
	return tmpStd;
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
    setXRange(2582);

    count = 0;
    state = CALIBRATION;
    printf("sqrt: %f\r\n",sqrt(953.8));
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
    		diff_x = charger.xTime - average_x;
    		diff_y = charger.yTime - average_y;
    		diff_z = charger.zTime - average_z;

    		printf("diff: %d \r\n", diff_x);
    		if((diff_x > RADIUS && diff_x > std_x) ||
    				(diff_y > RADIUS && diff_y > std_y) ||
    				(diff_z > RADIUS && diff_z > std_z)){
    			point1.x_pos = diff_x;
    			//point1.y_pos = diff_y;
    			//point1.z_pos = diff_z;
    			point1.y_pos = 100;
				point1.z_pos = 100;
    			queueHandler_pushPoint(&point1);
				queueHandler_draw();
    		}else{
    			printf("Out range\r\n");
    		}
    	}

    	if(count == MAX_COUNT){
    		average_x = (double)sum_x / (double)count;
    		average_y = (double)sum_y / (double)count;
    		average_z = (double)sum_z / (double)count;

    		std_x = calculateStd(dataset_x, count, average_x);
    		std_y = calculateStd(dataset_y, count, average_y);
    		std_z = calculateStd(dataset_z, count, average_z);

    		average_x += std_x;
			average_y += std_y;
			average_z += std_z;
    		printf("average: %f %f %f\r\n", average_x, average_y, average_z);
    		printf("std: %f %f %f\r\n", std_x, std_y, std_z);
    		state = FUNCTION;
    		count++;
    	}
    	printf("%lu %lu %lu\r\n", charger.xTime, charger.yTime, charger.zTime);
        charger.newDataFlag = 0;
    }
}
