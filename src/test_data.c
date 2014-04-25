/**************************************************************************************
	@file: test.c
	@brief: get test data from the data class
	@author: Zhen Jiang
	@created: April 1, 2014
**************************************************************************************/

#include "test_data.h"

int test_count = 0;

/**
  * this function reset the data counter
  * Paramters: void
  *
  * @return void
  */
void test_reset(){
	test_count = 0;
}

/**
  * get one test data our from data class
  * Paramters:
  *
  * @return the test data
  */
int test_getData(){
	if(data[test_count] == -1){
		return -1;
	}
	int tmp = data[test_count];
	test_count++;
	return data[test_count];
}

