 /******************************************************************************
 *@file: charger.h
 *
 *@brief: 
 *  - This is a the declarations for our charger class
 *    
 *    
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author:  Daniel Abel
 * @created: 03/21/14
 *
 *******************************************************************************/
  
#ifndef _CHARGER_H_
#define _CHARGER_H_
#include "constant.h"


 /***************** Definitions  ********************************************/
/* Any definitions that can be seen outside  */


/***************** Data Type Definitions ************************************/

/** template attributes
 */
typedef struct {
	STATE x_state;
	STATE y_state;
	STATE z_state;
	CAL_STATE_T calibration_state;
	unsigned long baseline_x;
	unsigned long baseline_y;
	unsigned long baseline_z;
	unsigned long range_x;
	unsigned long range_y;
	unsigned long range_z;
	unsigned long xTime; /** time for the x plate to charge */
	unsigned long yTime; /** time for the y plate to charge */
	unsigned long zTime; /** time for the z plate to charge */
	unsigned short newDataFlag; /** allows readers to know there is new position data **/
	unsigned short currentPlate;
	unsigned long total;
} charger_t;

/***************** Method Prototypes ****************************************/

/** Initialization for charger, inits interrupts for x, y, z. Inits timer.
 *
 * @param pThis  pointer to own object
 *
 * @return Zero on success, negative otherwise 
 */
void charger_init(charger_t *pThis);

/** starts the chargers polling loop **/
int charger_run(charger_t *pThis);

/** enable debug **/
void charger_debug_enable();

/**
 * Sets PORTF0 to input and enables interrupt
 */
void gpio_setInput_PORTF0();
/**
 * Sets PORTF1 to input and enables interrupt
 */
void gpio_setInput_PORTF1();
/**
 * Sets PORTF2 to input and enables interrupt
 */
void gpio_setInput_PORTF2();

/**
 * Sets PORTF0 to output and sets output to high
 */
void gpio_setOutput_PORTF0();
/**
 * Sets PORTF1 to output and sets output to high
 */
void gpio_setOutput_PORTF1();
/**
 * Sets PORTF2 to output and sets output to high
 */
void gpio_setOutput_PORTF2();
 #endif
