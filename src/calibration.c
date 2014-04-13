#include "calibration.h"
#include "constant.h"
#include "ssvep.h"

#define CALIBRATION	10
#define FUNCTION	11

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

long max_x = 0;
long max_y = 0;
long max_z = 0;

#define MAX_COUNT 50
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

void calibrate(charger_t* pThis){
	while(1){
		if(pThis->x_state == READY &&
				pThis->y_state == READY &&
				pThis->z_state == READY){

			//find range
			pThis->range_x = max_x - pThis->baseline_x;
			pThis->range_y = max_y - pThis->baseline_y;
			pThis->range_z = max_z - pThis->baseline_z;

			printf("Calibration Done\r\n");
			return;
		}
		if(ERROR == charger_run(pThis)){
			pThis->newDataFlag = 0;
			continue;
		}
		if(state == CALIBRATION){
			sum_x += pThis->xTime;
			sum_y += pThis->yTime;
			sum_z += pThis->zTime;

			dataset_x[count] = pThis->xTime;
			dataset_y[count] = pThis->yTime;
			dataset_z[count] = pThis->zTime;
			count++;
		}else{
			if(pThis->z_state != READY){
				if(max_z < pThis->zTime){
					max_z = pThis->zTime;
				}
			}else if(pThis->x_state != READY){
				if(max_x < pThis->xTime){
					max_x = pThis->xTime;
				}
			}else if(pThis->y_state != READY){
				if(max_y < pThis->yTime){
					max_y = pThis->yTime;
				}
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
			pThis->baseline_x = average_x;
			pThis->baseline_y = average_y;
			pThis->baseline_z = average_z;

			printf("average: %f %f %f\r\n", average_x, average_y, average_z);
			printf("std: %f %f %f\r\n", std_x, std_y, std_z);

			//state changed
			state = FUNCTION;

			count++;

			//enable push button interrupt
			*pGPIO_EN |= 1 << PUSHBUTTON_POSITION;//enable as GPIO
			*pGPIO_OE |= 1 << PUSHBUTTON_POSITION;//configure i/o
			*pGPIO_IN_INTE |= 1 << PUSHBUTTON_POSITION;//interrupt enable
		}
		printf("cali: %lu %lu %lu\r\n", pThis->xTime, pThis->yTime, pThis->zTime);
		pThis->newDataFlag = 0;
	}
}
