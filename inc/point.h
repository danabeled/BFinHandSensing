/**************************************************************************************
	@file: point.h
	@brief: Point handles all data related to each hand position read, is contained 
 		in queue
	@author: pstephen
	@created: March 21, 2014
**************************************************************************************/

#ifndef POINT_H
#define POINT_H


/*********************************Data Type Definitions****************************/

typedef struct point{
	unsigned int x_pos;
	unsigned int y_pos;
	unsigned int z_pos;

	struct point * nextPoint;
	struct point * prevPoint;
} point_t;


/********************************* Method Prototypes *******************************/
/**
 @param x position 
 @param y position
 @param z position
 @param pointer to point being initilized
 @param point_t* previous point in queue
 @param point_t* next point in queue
 @return status bit
*/
int point_init(point_t *, int, int, int, point_t *, point_t *);
/**
 @param point_t the point to be printed
 @return status
*/
int point_print(point_t *);
#endif

