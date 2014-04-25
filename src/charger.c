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
#include "test_data.h"
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

#define refresh 2*10000
#define resolution 8

int debug = DISABLE;

isrDisp_t pDispThis;

#define MAX 10
long xPoints[MAX];
int xcount = 1;
long yPoints[MAX];
int ycount = 1;
long zPoints[MAX];
int zcount = 1;

int divisor = 0;
 /*****************  Private Method Prototypes *********************************/
void setPinOutput(int position){
	*pGPIO_IN_INTE &= ~(1 << position);
	*pGPIO_OE |= (1 << position);
	*pGPIO_OUT &= ~(1 << position);
}
void setPinInput(int position, charger_t* pThis){
	*pGPIO_OE &= ~(1 << position | 1 << CAL_Z_PLATE | 1 << CAL_X_PLATE | 1 << CAL_Y_PLATE);
	*pGPIO_IN_INTE |= (1 << position | 1 << CAL_Z_PLATE | 1 << CAL_X_PLATE | 1 << CAL_Y_PLATE);
}
long charger_time(charger_t* charger){
	short position = charger ->currentPlate;
	unsigned long count = 0, total = 0;
	unsigned long current_time = 0;
	//start timer counting
	timer_start();
	//find the average of the charging time over refresh time
	while(current_time < refresh){
		charger->newDataFlag = 0;
		//set the pin output mode
		setPinOutput(position);
		//set the pin input mode
		setPinInput(position, charger);
		//see how many cycle pass before the pin is 1
		while(charger->newDataFlag != 1){
			count++;
		}
		setPinOutput(position);
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
	pThis->total = 0;
	pThis->x_state = NOT_READY;
	pThis->y_state = NOT_READY;
	pThis->z_state = NOT_READY;
	pThis->calibration_state = BASELINE;

	timer_init();
	printf("Charger init completed\r\n");
	test_reset();
}

/** Starts the chargers polling loop 
 *
 * @param pThis  pointer to own object
 *
 * @return Zero on success, negative otherwise 
 * */
int charger_run(charger_t *pThis) {
	//test code
//	pThis->xTime = test_getData();
//	pThis->yTime = 100;
//	pThis->zTime = 100;
//	return SUCCESSFUL;

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

	if(divisor < MAX){
		divisor++;
	}
	//measure x plate
	pThis->currentPlate = PLATE_X;
	pThis->xTime = charger_time(pThis);


	xPoints[xcount-1] = pThis->xTime;
	int i;
	int sum = 0;
	for(i = 0; i < divisor; i++){
		sum += xPoints[i];
	}
	pThis->xTime = sum / divisor;


	//printf("x \r\n");
	setPinOutput(PLATE_X);
	//measure y plate
	pThis->currentPlate = PLATE_Y;
	pThis->yTime = charger_time(pThis);
	setPinOutput(PLATE_Y);

	yPoints[xcount-1] = pThis->yTime;
	sum = 0;
	for(i = 0; i < divisor; i++){
		sum += yPoints[i];
	}
	pThis->yTime = sum / divisor;


	//printf("y \r\n");
	//measure z plate
	pThis->currentPlate = PLATE_Z;
	pThis->zTime = charger_time(pThis);
	setPinOutput(PLATE_Z);

	zPoints[xcount-1] = pThis->zTime;
	sum = 0;
	for(i = 0; i < divisor; i++){
		sum += zPoints[i];
	}
	pThis->zTime = sum / divisor;

	xcount++;
	if(xcount == MAX){
		xcount = 1;
	}
	//printf("z \r\n");
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

