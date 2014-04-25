#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#define ERROR -1 // eror value
#define SUCCESSFUL 0 //success value

#define RADIUS 8 //radius of circle drawn on the LCD screen

//these are used for calibration
#define CAL_X_PLATE 4 //interrupt pin for x plate when the plate finds the max value
#define CAL_Y_PLATE 5 //interrupt pin for y plate when the plate finds the max value
#define CAL_Z_PLATE 3 //interrupt pin for z plate when the plate finds the max value

#define MAX_TIMER_VALUE 20000 //max charging time, greater values are invalid

//charger state
typedef enum STATE{
	READY,
	NOT_READY
} STATE;

//calibration state machine
typedef enum CAL_STATE {
	BASELINE,//finding baseline
	CHARGING_X,//finding range for x
	CHARGING_Y,//finding range for y
	CHARGING_Z,//finding range for z
	CAL_DONE//done calibration
} CAL_STATE_T;

#endif
