/**************************************************************************************
	@file: test.h
	@brief: get test data from the data class
	@author: Zhen Jiang
	@created: April 1, 2014
**************************************************************************************/

#ifndef TEST_H
#define TEST_H

#include "data.h"
/**
  * this function reset the data counter
  * Paramters: void
  *
  * @return void
  */
void test_reset();
/**
  * get one test data our from data class
  * Paramters:
  *
  * @return the test data
  */
int test_getData();

#endif

