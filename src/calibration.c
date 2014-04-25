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
const double NUM_STD = 0.5;
unsigned long sum_x = 0;
unsigned long sum_y = 0;
unsigned long sum_z = 0;

unsigned long max_x = 0;
unsigned long max_y = 0;
unsigned long max_z = 0;

#define MAX_COUNT 300
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
	}
	double tmpVariance = ((double)tmpSum)/datalen;
	double tmpStd = sqrt(tmpVariance);
	return tmpStd;
}

void calibrate(charger_t * pThis) {
	printf("\r\n Entering Baseline Calibration \r\n");
	queueHanlder_drawTextAtCenter("Finding baseline...");
	pThis->calibration_state = BASELINE;
	pThis->z_state = NOT_READY;
	pThis->y_state = NOT_READY;
	pThis->x_state = NOT_READY;
	count = 0;
	while (1) {
		// Reset the charger obj if charger_run has an error
		if(ERROR == charger_run(pThis)){
			pThis->newDataFlag = 0;
			continue;
		}
		if(pThis->xTime > MAX_TIMER_VALUE ||
				pThis->yTime > MAX_TIMER_VALUE ||
				pThis->zTime > MAX_TIMER_VALUE){
			//ignore these values
			continue;
		}
		if(pThis->xTime == 0 ||
				pThis->yTime == 0 ||
				pThis->zTime == 0){
			//ignore these values
			continue;
		}

		// Calibration Procedure
		switch (pThis->calibration_state) {
			// Calibration Step 1: Find Baseline 
			// Get average timer counts for
			// when 3D hand sensor is empty
			case BASELINE:
				sum_x += pThis->xTime;
				sum_y += pThis->yTime;
				sum_z += pThis->zTime;

				dataset_x[count] = pThis->xTime;
				dataset_y[count] = pThis->yTime;
				dataset_z[count] = pThis->zTime;
				count++;

				printf("cali: %lu %lu %lu %d\r\n", pThis->xTime, pThis->yTime, pThis->zTime, count);

				if (count == MAX_COUNT) {

					average_x = (double)sum_x / (double)count;
					average_y = (double)sum_y / (double)count;
					average_z = (double)sum_z / (double)count;

					std_x = calculateStd(dataset_x, count, average_x);
					std_y = calculateStd(dataset_y, count, average_y);
					std_z = calculateStd(dataset_z, count, average_z);

					average_x += NUM_STD * std_x;
					average_y += NUM_STD * std_y;
					average_z += NUM_STD * std_z;

					pThis->baseline_x = average_x;
					pThis->baseline_y = average_y;
					pThis->baseline_z = average_z;

					printf("average:::: %f %f %f\r\n", average_x, average_y, average_z);
					printf("std:::: %f %f %f\r\n", std_x, std_y, std_z);

					//state changed
					pThis->calibration_state = CHARGING_Z;
					printf("\r\n Entering Z-Plate Calibration \r\n");
					queueHanlder_drawTextAtCenter("Please calibrate Z-Plate...");
					pThis->z_state = NOT_READY;
				}
				break;

			// Calibration Step 2: Z-Plate 
			// Get the max timer counts on 
			// the z-plate when a hand is right
			// over it
			case CHARGING_Z:
			
				if(max_z < pThis->zTime){
					max_z = pThis->zTime;
				}

				printf("Z current range: %d\r\n", max_z - pThis->baseline_z);

				if (pThis->z_state == READY) {
					pThis->calibration_state = CHARGING_X;
					printf("\r\n Entering X-Plate Calibration \r\n");
					queueHanlder_drawTextAtCenter("Please calibrate X-Plate...");
					pThis->x_state = NOT_READY;
				}
				break;

			// Calibration Step 3: X-Plate 
			// Get the max timer counts on 
			// the X-plate when a hand is right
			// over it
			case CHARGING_X:

				if(max_x < pThis->xTime){
					max_x = pThis->xTime;
				}

				printf("x range: %d z range: %d\r\n", max_x - pThis->baseline_x,
						max_z - pThis->baseline_z);

				if (pThis->x_state == READY) {
					pThis->calibration_state = CHARGING_Y;
					printf("\r\n Entering Y-Plate Calibration \r\n");
					queueHanlder_drawTextAtCenter("Please calibrate Y-Plate...");
					pThis->y_state = NOT_READY;
				}
				break;

			// Calibration Step 4: Y-Plate 
			// Get the max timer counts on 
			// the y-plate when a hand is right
			// over it
			case CHARGING_Y:

				if(max_y < pThis->yTime){
					max_y = pThis->yTime;
				}
				printf("y range: %d x range: %d z range: %d\r\n", max_y - pThis->baseline_y,
						max_x - pThis->baseline_x,
						max_z - pThis->baseline_z);

				if (pThis->y_state == READY) {
					pThis->calibration_state = CAL_DONE;

					//before exiting we need to calculate the range for each plate
					pThis->range_x = max_x - pThis->baseline_x;
					pThis->range_y = max_y - pThis->baseline_y;
					pThis->range_z = max_z - pThis->baseline_z;
					//calibration done here, otherwise we need another sample to exit the
					//calibration mode
					printf("\r\n Calibration Procedure Complete \r\n");
					return;
				}
				break;

			default:
				break;
		}
		pThis->newDataFlag = 0; // Reset charger obj
	}
}

void calibrate_baseline(charger_t* pThis){
	count = 0;
	sum_x = 0;
	sum_y = 0;
	sum_z = 0;
	queueHanlder_drawTextAtCenter("Calculating baseline...");
	while (1) {
		// Reset the charger obj if charger_run has an error
		if(ERROR == charger_run(pThis)){
			pThis->newDataFlag = 0;
			continue;
		}
		if(pThis->xTime > MAX_TIMER_VALUE ||
				pThis->yTime > MAX_TIMER_VALUE ||
				pThis->zTime > MAX_TIMER_VALUE){
			//ignore these values
			continue;
		}
		printf("cali: %lu %lu %lu\r\n", pThis->xTime, pThis->yTime, pThis->zTime);

		sum_x += pThis->xTime;
		sum_y += pThis->yTime;
		sum_z += pThis->zTime;

		dataset_x[count] = pThis->xTime;
		dataset_y[count] = pThis->yTime;
		dataset_z[count] = pThis->zTime;
		count++;

		printf("count: %d\r\n", count);

		if (count == MAX_COUNT) {

			average_x = (double)sum_x / (double)count;
			average_y = (double)sum_y / (double)count;
			average_z = (double)sum_z / (double)count;

			std_x = calculateStd(dataset_x, count, average_x);
			std_y = calculateStd(dataset_y, count, average_y);
			std_z = calculateStd(dataset_z, count, average_z);

			average_x += NUM_STD * std_x;
			average_y += NUM_STD * std_y;
			average_z += NUM_STD * std_z;

			pThis->baseline_x = average_x;
			pThis->baseline_y = average_y;
			pThis->baseline_z = average_z;

			printf("average:::: %f %f %f\r\n", average_x, average_y, average_z);
			printf("std:::: %f %f %f\r\n", std_x, std_y, std_z);

			//state changed
			pThis->calibration_state = CHARGING_Z;
			queueHanlder_drawTextAtCenter("Baseline calculation completed...");
			return;
		}
	}
}
