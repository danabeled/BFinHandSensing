#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#define ERROR -1
#define SUCCESSFUL 0

#define RADIUS 8

#define CAL_X_PLATE 4
#define CAL_Y_PLATE 5
#define CAL_Z_PLATE 3

#define MAX_TIMER_VALUE 20000

typedef enum STATE{
	READY,
	NOT_READY
} STATE;

typedef enum CAL_STATE {
	BASELINE,
	CHARGING_X,
	CHARGING_Y,
	CHARGING_Z,
	CAL_DONE
} CAL_STATE_T;

#endif
