/**************************************************************************************
	@file: point.h
	@brief: Point handles all data related to each hand position read
	@author: pstephen
	@created: March 21, 2014
**************************************************************************************/

#ifndef POINT_H
#define POINT_H

/********************************* 1) Definiation ************************************/


/********************************* 2) Data Type Definitions****************************/

typedef struct {
	unsigned int x_pos;
	unsigned int y_pos;
	unsigned int z_pos;
} point_t;


/********************************* 3) Global Data Declaration**************************/
/********************************* 4) Method Prototypes *******************************/
/**
 @param x position 
 @param y position
 @param z position
 @param pointer to point being initilized
 @return status bit
*/
int point_init(point_t *, int, int, int);

#endif

