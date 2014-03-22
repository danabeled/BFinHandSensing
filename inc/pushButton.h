/**************************************************************************
	@file: pushButton.h 
	@brief:Deals with all pushbutton functionality  
 		in queue
	@author: pstephen
	@created: March 22, 2014
**************************************************************************/

#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H


/*********************************Data Type Definitions****************************/

typedef struct pushButton{
} pushButton_t;


/********************************* Method Prototypes *******************************/
/**
 @param point_t* next point in queue
 @return status bit
*/
int pushButton_init(pushButton_t *);

#endif

