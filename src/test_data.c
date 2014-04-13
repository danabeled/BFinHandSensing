/**************************************************************************************
	@file: point.h
	@brief: Point handles all data related to each hand position read, is contained 
 		in queue
	@author: pstephen
	@created: March 21, 2014
**************************************************************************************/

#include "test_data.h"

int test_count = 0;

void test_reset(){
	test_count = 0;
}

int test_getData(){
	if(data[test_count] == -1){
		return -1;
	}
	int tmp = data[test_count];
	test_count++;
	return data[test_count];
}

