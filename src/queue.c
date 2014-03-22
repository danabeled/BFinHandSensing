/****************************************************************************
	@file: queue.c 
	@breif: 
	@author: pstephen
	@created: March 21, 2014
****************************************************************************/

#include "point.h"
#include "queue.h"
#include <stdlib.h>

/*********************************Method Implementations********************/
/**
 @param pThis: queue instance being modified
 @return: status bit
*/
int STATUS;
int queue_init(queue_t *pThis){
	pThis->queueSize = 0;
	int SUCCESSFUL = 1;
	return SUCCESSFUL;
}
 
/*
 @param pThis: queue instance being added to
 @param x: x position
 @param y: y position
 @param z: z position
 @return status bit
*/
int queue_addPoint(queue_t *pThis, int x, int y, int z){
	point_t *newPoint = (point_t *) malloc(sizeof(point_t));
	if(pThis->queueSize == 0){ 
		//new point becomes the first and last element if the list is empty
		pThis->firstElement = newPoint; 
		pThis->lastElement = newPoint;
		STATUS = point_init(newPoint, x, y, z, NULL, NULL);
		if(STATUS != 0){ return -1; }
	}else{
		STATUS = point_init(newPoint, x, y ,z,pThis->lastElement, NULL);
		if(STATUS != 0){ return -1; }
		point_t * temp = pThis->lastElement;
		newPoint->nextPoint = temp;
		temp->prevPoint = newPoint;
		pThis->lastElement = newPoint;
	}
	pThis->queueSize++;

}
/**
 @param pThis: queue instance being modified
 @return: next element
*/
point_t * queue_getPoint(queue_t *pThis){
	if(pThis->queueSize == 0){ return NULL; }
		point_t* temp  = pThis->firstElement;
		pThis->firstElement = temp->prevPoint;
		pThis->queueSize--;
	return temp;
}
		
int queue_print(queue_t *pThis){
	point_t *itr = pThis->firstElement;
	while(itr != NULL){
		printf("x = %u , y = %u, z = %u \n", itr->x_pos, itr->y_pos, itr->z_pos);
		itr = itr->prevPoint;
	}
	printf("Queue size is: %d \n", pThis->queueSize);
	return 1;
}

int queue_clear(queue_t *pThis) {
  point_t * itr = pThis->lastElement;

  while (itr != NULL) {
    free(itr);
    itr = itr->prevPoint;
  }
  
  pThis->firstElement = NULL;
  pThis->lastElement = NULL;
  
  return queue_init(pThis);
}
