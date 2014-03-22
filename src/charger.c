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
#include "constant.h"
#include "cdefBF52x_base.h"

/***************** Private Defines *********************************************/
#define CHARGER_G_BITS 0x1C
#define PLATE_X 0x4
#define PLATE_Y 0x16
#define PLATE_Z 0x8

 /*****************  Private Method Prototypes *********************************/
void charger_disable_input()
{
	*pPORTGIO_DIR |= CHARGER_G_BITS;
	*pPORTGIO_INEN &= ~(CHARGER_G_BITS);
}

void charger_enable_input()
{
	*pPORTGIO_DIR &= ~(CHARGER_G_BITS);
	*pPORTGIO_INEN |= CHARGER_G_BITS;
}


 /*****************  Methods Implementations **********************************/

/** Initialization for template
 *
 * @param pThis  pointer to own object
 *
 * @return Zero on success, negative otherwise 
 */
void charger_init(charger_t *pThis) {
	
	/* 	initialize Port G's 2-4 bits direction, and clear
		the bits. Set x, y, z, and number of plates
		charged to zero.  */
	*pPORTGIO_DIR |= CHARGER_G_BITS;
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
void charger_run(charger_t *pThis) {
	charger_discharge(pThis);
	charger_charge(pThis);
}

/** set GPIO bits to output and reintialize timer **/
int charger_discharge(charger_t *pThis){
	charger_disable_input();
    timer_stop();
    pThis->xCharged = 0;
    pThis->yCharged = 0;
    pThis->zCharged = 0;
    pThis->numPlatesCharged = 0;
    return SUCCESSFUL;
}

/** set GPIO bits to input, start timer, poll until 3 plates
	are all charged **/

int charger_charge(charger_t *pThis) {
	while(pThis->newDataFlag==1)
	{
		/*do nothing*/
	}
	/* change GPIO to input */
	charger_enable_input();
	/* start timer */
	if(ERROR == timer_start())
	{
		return ERROR;
	}

	while(pThis->numPlatesCharged!=3)
	{

		if(pThis->xCharged==0 && (*pPORTGIO&PLATE_X==PLATE_X))
		{
			if(ERROR == timer_getValue(&(pThis->xTime)))
				return ERROR; 
			pThis->numPlatesCharged++;
			pThis->xCharged=1;
		}
		if(pThis->yCharged==0 && (*pPORTGIO&PLATE_Y==PLATE_Y))
		{
			if(ERROR == timer_getValue(&(pThis->yTime)))
				return ERROR; 
			pThis->numPlatesCharged++;
			pThis->yCharged=1;
		}
		if(pThis->zCharged==0 && (*pPORTGIO&PLATE_Z==PLATE_Z))
		{
			if(ERROR == timer_getValue(&(pThis->zTime)))
				return ERROR; 
			pThis->numPlatesCharged++;
			pThis->zCharged=1;
		}
	}
	pThis->newDataFlag=1;
	return SUCCESSFUL;
}
