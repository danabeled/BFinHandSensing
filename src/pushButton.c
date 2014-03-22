/**************************************************************************
	@file: pushButton.h 
	@brief:Deals with all pushbutton functionality  
 		in queue
	@author: pstephen
	@created: March 22, 2014
***************************************************************************/

#include "pushButton.h"
int STATUS;
/********************************Method Implementations********************/
int pushButton_init(pushButton_t *pThis){
    extio_input pb1_itr = EXTIO_PB1_HIGH;
    extio_input pb0_itr = EXTIO_PB0_HIGH;

    void (*pCalibration) (void*) = pushButton_calibration;
    void (*pClear) (void*) = pushButton_clear;
    
    STATUS = extio_callbackRegister(pb1_itr, pCalibration, pThis);
    if(STATUS != PASS){ return FAIL; }
    
    STATUS = extio_callbackRegister(pb0_itr, pClear, pThis);
    if(STATUS != PASS){ return FAIL; }
    
    return PASS; 

}

int pushButton_calibration(void *pThis){
    return SUCCESSFUL;
}

int pushButton_clear(void *pThis){
    return SUCESSFUL;
}
