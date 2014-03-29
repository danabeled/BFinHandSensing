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



 /***************** Definitions  ********************************************/
/* Any definitions that can be seen outside  */


/***************** Data Type Definitions ************************************/

/** template attributes
 */
typedef struct {
	unsigned int xTime; /** time for the x plate to charge */
	unsigned int yTime; /** time for the y plate to charge */
	unsigned int zTime; /** time for the z plate to charge */
	unsigned int numPlatesCharged; /** number of plates curerntly charged **/
	unsigned short xCharged;
	unsigned short yCharged;
	unsigned short zCharged;
	unsigned short newDataFlag; /** allows readers to know there is new position data **/
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
void charger_run(charger_t *pThis);

/** enable debug **/
void charger_debug_enable();

/** disable debug **/
void charger_debug_disable();

 #endif
