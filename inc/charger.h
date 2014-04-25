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
 * @updated: Zhen Jiang, 04/01/2014
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
	STATE x_state;//x plate state
	STATE y_state;//y plate state
	STATE z_state;//z plate state
	CAL_STATE_T calibration_state;//charger calibration state
	unsigned long baseline_x;//x plate time baseline
	unsigned long baseline_y;//y plate time baseline
	unsigned long baseline_z;//z plate time baseline
	unsigned long range_x;//x plate time range
	unsigned long range_y;//y plate time range
	unsigned long range_z;//z plate time range
	unsigned long xTime; /** time for the x plate to charge */
	unsigned long yTime; /** time for the y plate to charge */
	unsigned long zTime; /** time for the z plate to charge */
	unsigned short newDataFlag; /** allows readers to know there is new position data **/
	unsigned short currentPlate;//current charging plate
	unsigned long total;//total number of charging
} charger_t;

/***************** Method Prototypes ****************************************/

/** Initialization for charger, inits interrupts for x, y, z. Inits timer.
 *
 * @param pThis  pointer to own object
 *
 * @return Zero on success, negative otherwise 
 */
void charger_init(charger_t *pThis);

/** charge the charge
 *
 * @param pThis  pointer to own object
 *
 * @return Zero on success, negative otherwise 
 */
int charger_run(charger_t *pThis);

/** enable debug **/
void charger_debug_enable();

 #endif
