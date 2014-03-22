/**************************************************************************************
	@file: queue.h
	@brief: queue handles all point objects in the form of a linked list
	@author: pstephen
	@created: March 21, 2014
**************************************************************************************/

#ifndef QUEUE_H
#define QUEUE_H


#include "point.h"
/*********************************Data Type Definitions****************************/

typedef struct queue{
	point_t *firstElement;
	point_t *lastElement;

	int queueSize;
} queue_t;


/*********************************Method Prototypes *******************************/
/**
 @param queue instance to be initilized
 @return status bit
*/
int queue_init(queue_t *);
/**
 @param queue_t: queue instance being added to
 @param x: x position
 @param y: y position
 @param z: z position
 @retun status
*/
int queue_addPoint(queue_t *,int, int, int);
/**
 @param queue_t queue being referenced
 @return the next element in the queue, returns NULL if the queue is empty
*/
point_t* queue_getPoint(queue_t *);
/**
 @param queue to be printed
 @return status
*/
int queue_print(queue_t *);

#endif

