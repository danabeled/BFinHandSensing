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
//the following values are for the PORT F IMPLEMENTATION **FOR NOW**
#define CHARGER_G_BITS 0x700
#define PLATE_X 8
#define PLATE_Y 9
#define PLATE_Z 10

#define ENABLE 1
#define DISABLE 0

int debug = DISABLE;
 /*****************  Private Method Prototypes *********************************/
void charger_disable_input()
{
	*pPORTFIO_INEN &= ~(CHARGER_G_BITS);
	*pPORTFIO_DIR |= CHARGER_G_BITS;
	*pPORTFIO &= ~(CHARGER_G_BITS);
}

void charger_enable_input()
{
	*pPORTFIO_DIR &= ~(CHARGER_G_BITS);
	*pPORTFIO_INEN |= CHARGER_G_BITS;
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
	*pPORTF_FER &= ~(CHARGER_G_BITS);
	*pPORTFIO_DIR &= 0;
	*pPORTFIO_CLEAR |= 0xFFFF;
	*pPORTFIO_INEN &= ~(CHARGER_G_BITS);

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
const char *byte_to_binary(int x)
{
    static char b[17];
    b[0] = '\0';

    int z;
    for (z = 65536; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

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

	while(pThis->numPlatesCharged < 3)
	{
		short val = *pPORTFIO;
		if(pThis->xCharged == 0 && (val >> PLATE_X) % 2)
		{
			if(ERROR == timer_getValue(&(pThis->xTime)))
				return ERROR;
			pThis->numPlatesCharged++;
			pThis->xCharged=1;
			if(debug == ENABLE)
				printf("X time:%d\r\n",pThis->xTime);
		}
		if(pThis->yCharged==0 && (val >> PLATE_Y) % 2)
		{
			if(ERROR == timer_getValue(&(pThis->yTime)))
				return ERROR;
			pThis->numPlatesCharged++;
			pThis->yCharged=1;
			if(debug == ENABLE)
				printf("Y time:%d\r\n",pThis->yTime);
		}
		if(pThis->zCharged==0 && (val >> PLATE_Z) % 2)
		{
			if(ERROR == timer_getValue(&(pThis->zTime)))
				return ERROR;
			pThis->numPlatesCharged++;
			pThis->zCharged=1;
			if(debug == ENABLE)
				printf("Z time:%d\r\n",pThis->zTime);
		}
	}
	pThis->newDataFlag=1;
	return SUCCESSFUL;
}

/** enable debug **/
void charger_debug_enable(){
	debug = ENABLE;
}

/** disable debug **/
void charger_debug_disable(){
	debug = DISABLE;
}
