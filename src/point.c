/****************************************************************************
:seti:	@file: point.c 
	@breif: Point handles all data related to each hand position read
	@author: pstephen
	@created: March 21, 2014
****************************************************************************/

#include "point.h"

int SUCCESSFUL = 0;
/*********************************Method Implementations********************/
/**
 @param x: x position
 @param y: y position
 @param z: z position
 @param pThis: pointer to point instance being initlized
 @return: status bit
*/
int point_init(point_t *pThis, int x, int y, int z,point_t *Next,point_t *Prev){
	pThis->x_pos = x;
	pThis->y_pos = y;
	pThis->z_pos = z;
	
	pThis->nextPoint = Next;
	pThis->prevPoint = Prev;
	
	int queueSize;
	return SUCCESSFUL;
}
int point_print(point_t *pThis){
	printf("Point is: x = %u, y = %u, z = %u \r\n", pThis->x_pos, pThis->y_pos,
		 pThis->z_pos);
	return SUCCESSFUL;
}
