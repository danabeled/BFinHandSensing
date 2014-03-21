/****************************************************************************
	@file: point.c 
	@breif: Point handles all data related to each hand position read
	@author: pstephen
	@created: March 21, 2014
****************************************************************************/

#include <point.h>

/*********************************Method Implementations********************/
/**
 @param x: x position
 @param y: y position
 @param z: z position
 @param pThis: pointer to point instance being initlized
 @return: status bit
*/
int point_init(point_t *pThis, int x, int y, int z){
	pThis->x_pos = x;
	pThis->y_pos = y;
	pThis->z_pos = z;
	
	return SUCCESSFUL;
}

