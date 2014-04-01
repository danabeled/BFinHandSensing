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
 /*****************  Private Method Prototypes *********************************/
void setPinOutput(int position){
	//disable input driver
	pPORTIO_INEN &= ~(1 << position);
	//enable output direction
	pPORTIO_DIR |= 1 << position;
	//set output voltage to 0
	pPORTIO_CLEAR |= 1 << position;
}
void setPinInput(int position){
	//enable input direction
	pPORTIO_DIR &= ~(1 << position);
	//enable input driver
	pPORTIO_INEN |= 1 << position;
}
long charger_time(int position){
	unsigned long count = 0, total = 0;
	unsigned long current_time = 0;
	//start timer counting
	timer_start();
	//find the average of the charging time over refresh time
	while(current_time < refresh){
		//set the pin output mode
		setPinOutput(position);
		//set the pin input mode
		setPinInput(position);
		//see how many cycle pass before the pin is 1
		while((pPORTIO >> position) % 2 == 0){
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
	pPORT_FER &= ~(CHARGER_G_BITS);
	pPORTIO_DIR &= ~(CHARGER_G_BITS);
	pPORTIO_CLEAR |= CHARGER_G_BITS;
	pPORTIO_INEN &= ~(CHARGER_G_BITS);

	pThis->xTime = 0;
	pThis->yTime = 0;
	pThis->zTime = 0;
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
	if(pThis->newDataFlag==1) {return ERROR;}

	if(ERROR == timer_start()){return ERROR;}

	//measure x plate
	pThis->xTime = charger_time(PLATE_X);

	//measure y plate
	pThis->yTime = charger_time(PLATE_Y);

	//measure z plate
	pThis->zTime = charger_time(PLATE_Z);

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
