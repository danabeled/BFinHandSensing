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
#include <ssvep.h>
#include "fpgadab.h"
/***************** Private Defines *********************************************/
//the following values are for the PORT F IMPLEMENTATION **FOR NOW**
#define CHARGER_G_BITS 0x700

#define PLATE_X 0
#define PLATE_Y 1
#define PLATE_Z 2

#define pPORTIO_INEN *pPORTFIO_INEN
#define pPORTIO_DIR *pPORTFIO_DIR
#define pPORTIO_CLEAR *pPORTFIO_CLEAR
#define pPORT_FER *pPORTF_FER
#define pPORTIO *pPORTFIO


#define ENABLE 1
#define DISABLE 0

#define refresh 2*1000000
#define resolution 8

int debug = DISABLE;

isrDisp_t pDispThis;

 /*****************  Private Method Prototypes *********************************/
void setPinOutput(int position){
	*pGPIO_IN_INTE &= ~(1 << position);
	*pGPIO_OE |= (1 << position);
	*pGPIO_OUT &= ~(1 << position);
}
void setPinInput(int position){
	*pGPIO_OE &= ~(1 << position);
	*pGPIO_IN_INTE |= (1 << position);
}
long charger_time(int position, charger_t* charger){
	unsigned long count = 0, total = 0;
	unsigned long current_time = 0;
	//start timer counting
	timer_start();
	//find the average of the charging time over refresh time
	while(current_time < refresh){
		//set the pin output mode
		setPinOutput(position);
		charger->newDataFlag = 0;
		//set the pin input mode
		setPinInput(position);
		//see how many cycle pass before the pin is 1
		while(charger->newDataFlag != 1){
			count++;
		}
		//number of measure
		total++;
		//get pass time
		timer_getValue(&current_time);
	}
	//stop the timer
	timer_stop();
	return (count << resolution) / total;
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

	int ret = isrDisp_init(&pDispThis);
	if(0 != ret){
		printf("isrdisp init failed.\r\n");
		return;
	}
	ret = fpgadab_init(pThis, &pDispThis);
	if(0 != ret){
		printf("fpga init failed. \r\n");
		return;
	}
	/* 	call timer's initialization function 		*/

	pThis->xTime = 0;
	pThis->yTime = 0;
	pThis->zTime = 0;
	pThis->newDataFlag = 0;

	timer_init();
	printf("Charger init completed\r\n");
}

/** Starts the chargers polling loop 
 *
 * @param pThis  pointer to own object
 *
 * @return Zero on success, negative otherwise 
 * */
int charger_run(charger_t *pThis) {
	return charger_charge(pThis);
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

	if(ERROR == timer_start()){return ERROR;}

	//measure x plate
	pThis->xTime = charger_time(PLATE_X, pThis);

	//measure y plate
	pThis->yTime = charger_time(PLATE_Y, pThis);

	//measure z plate
	pThis->zTime = charger_time(PLATE_Z, pThis);

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

