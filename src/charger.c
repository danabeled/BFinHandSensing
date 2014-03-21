/******************************************************************************
 *@file: charger.c
 *
 *@brief: 
 *  - Implementation of charging code to sense time to charge sensors
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author:  Daniel Abel
 * @created: 03/21/14
 *
 *******************************************************************************/

#include "charger.h"
#include "timer.h"

/***************** Private Defines *****************************/
#define CHARGER_G_BITS 0x1C

 /*****************  Methods Implementations **********************************/

/** Initialization for template
 *
 * @param pThis  pointer to own object
 *
 * @return Zero on success, negative otherwise 
 */
int charger_init(template_t *pThis) {
	
	/* 	initialize Port G's 2-4 bits direction, and clear
		the bits. Set x, y, z, and number of plates
		charged to zero.  */
	*pPORTGIO_DIR &= ~(CHARGER_G_BITS);
	*pPORTGIO_CLEAR |= CHARGER_G_BITS;
	pThis->xTime = 0;
	pThis->yTime = 0;
	pThis->zTime = 0;
	pThis->numPlatesCharged = 0;
	pThis->newDataFlag = 0;

	/* 	call timer's initialization function 		*/
	timer_init();
}

/** Starts the chargers polling loop 
 *
 * @param pThis  pointer to own object
 *
 * @return Zero on success, negative otherwise 
 * */
int charger_run(charger_t *pThis); {
	charger_discharge(pThis);
	charger_charge(pThis);
}

/** set GPIO bits to output and reintialize timer **/
int charger_discharge(charger_t *pThis){
    timer_stop();

}

/** set GPIO bits to input, start timer, poll until 3 plates
	are all charged **/

int charger_charge(charger_t *pThis); {
	/* change GPIO to input */
	*pPORTGIO_DIR = CHARGER_G_BITS;
	if(ERROR == timer_start())
	{
		return ERROR;
	}
}
