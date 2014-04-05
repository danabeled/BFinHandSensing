/**
 *@file ssvep.h
 *
 *@brief
 *  - Introduction lab for the TLL6527 platform
 *    - SSVEP
 *
 *
 * Target:   TLL6527v1-1
 * Compiler:
 *
 *@author    Shen Feng, ECE, Northeastern University
 *@Last change: $Id$
 *
 *******************************************************************************/
#ifndef _SSVEP_H_ 
#define _SSVEP_H_

/**
 * define the number of SSVEP led arrays
 */
#define SSVEP_MAX 4
/**
* define pattern length, length/16. default value is 16, which is 256 bits
*/
#define SSVEP_PATTERN_LEN 16

/**
 * GPIO enable signal, 16 bits maps to corresponding pins
 */
#define GPIO_EN (0x20220000)
#define pGPIO_EN (unsigned short volatile*)GPIO_EN

/**
 * GPIO output enable, set 1 to output, 0 for input
 */
#define GPIO_OE (0x20221000)
#define pGPIO_OE (unsigned short volatile*) GPIO_OE

/**
 * GPIO input buffer, storing the input value.
 * Corresponding bits are valid only when GPIO_OE is set to 0
 */
#define GPIO_IN (0x20222000)
#define pGPIO_IN (unsigned short volatile*) GPIO_IN

/**
 * GPIO output buffer, storing the output value.
 * Corresponding bits are valid only when GPIO_OE is set to 1
 */
#define GPIO_OUT (0x20222000)
#define pGPIO_OUT (unsigned short volatile*) GPIO_OUT

/**
 * GPIO input interrupt enable
 */

#define GPIO_IN_INTE (0x20223000)
#define pGPIO_IN_INTE (unsigned short volatile*) GPIO_IN_INTE

/**
 * GPIO output interrupt enable. Configure GPIO_OUT_EDGE first, then set this MMR
 */
#define GPIO_OUT_INTE (0x20224000)
#define pGPIO_OUT_INTE (unsigned short volatile*) GPIO_OUT_INTE
/**
 * GPIO output edge trigger enable, 1 for edge, 0 for level
 */
#define GPIO_OUT_EDGE (0x20225000)
#define pGPIO_OUT_EDGE (unsigned short volatile*) GPIO_OUT_EDGE

/**
 * SSVEP Mask register, low 4 bits valid in the case of 4 ssvep channels, set 1 to enable
 */
#define SSVEP_MASK (0x2021f000)
#define pSSVEP_MASK (unsigned short volatile*) SSVEP_MASK

/**
 * SSVEP output enable register, set 1 to enable, set 0 to disable
 */
#define SSVEP_OEN (0x2021e000)
#define pSSVEP_OEN (unsigned short volatile*) SSVEP_OEN

/**
 * SSVEP0 base register
 */
#define SSVEP0_BASE (0x20210000)
#define pSSVEP0_BASE (unsigned short volatile*) SSVEP0_BASE

/**
 * SSVEP1 base register
 */
#define SSVEP1_BASE (0x20211000)
#define pSSVEP1_BASE (unsigned short volatile*) SSVEP1_BASE

/**
 * SSVEP2 base register
 */
#define SSVEP2_BASE (0x20212000)
#define pSSVEP2_BASE (unsigned short volatile*) SSVEP2_BASE

/**
 * SSVEP3 base register
 */
#define SSVEP3_BASE (0x20213000)
#define pSSVEP3_BASE (unsigned short volatile*) SSVEP3_BASE

/**
 * SSVEP0 pattern register, 0 to 7 stands for 8*16 bits pattern, LSB byte first
 */
#define OFFSET_PATTERN_0 (0x0000)
#define OFFSET_PATTERN_1 (0x0010)
#define OFFSET_PATTERN_2 (0x0020)
#define OFFSET_PATTERN_3 (0x0030)
#define OFFSET_PATTERN_4 (0x0040)
#define OFFSET_PATTERN_5 (0x0050)
#define OFFSET_PATTERN_6 (0x0060)
#define OFFSET_PATTERN_7 (0x0070)
#define OFFSET_PATTERN_8 (0x0080)
#define OFFSET_PATTERN_9 (0x0090)
#define OFFSET_PATTERN_a (0x00a0)
#define OFFSET_PATTERN_b (0x00b0)
#define OFFSET_PATTERN_c (0x00c0)
#define OFFSET_PATTERN_d (0x00d0)
#define OFFSET_PATTERN_e (0x00e0)
#define OFFSET_PATTERN_f (0x00f0)

/**
 * SSVEP0 frequency control word, frequency = MMR_value / 10, 0.1Hz resolution, from 0.1Hz to 409.6Hz
 */
#define OFFSET_FCW (0x0100)

/**
 * SSVEP0 brightness register, from 1 to 4999, dimmest to brightest
 */
#define OFFSET_BR (0x0200)

/**
 * SSVEP0 pattern length register, from 1 to 256
 */
#define OFFSET_PATTERN_LEN (0x0300)




//
//#define SSVEP0_PATTERN_0 (0x20210000)
//#define pSSVEP0_PATTERN_0 (unsigned short volatile*) SSVEP0_PATTERN_0
//
//#define SSVEP0_PATTERN_1 (0x20210010)
//#define pSSVEP0_PATTERN_1 (unsigned short volatile*) SSVEP0_PATTERN_1
//
//#define SSVEP0_PATTERN_2 (0x20210020)
//#define pSSVEP0_PATTERN_2 (unsigned short volatile*) SSVEP0_PATTERN_2
//
//#define SSVEP0_PATTERN_3 (0x20210030)
//#define pSSVEP0_PATTERN_3 (unsigned short volatile*) SSVEP0_PATTERN_3
//
//#define SSVEP0_PATTERN_4 (0x20210040)
//#define pSSVEP0_PATTERN_4 (unsigned short volatile*) SSVEP0_PATTERN_4
//
//#define SSVEP0_PATTERN_5 (0x20210050)
//#define pSSVEP0_PATTERN_5 (unsigned short volatile*) SSVEP0_PATTERN_5
//
//#define SSVEP0_PATTERN_6 (0x20210060)
//#define pSSVEP0_PATTERN_6 (unsigned short volatile*) SSVEP0_PATTERN_6
//
//#define SSVEP0_PATTERN_7 (0x20210070)
//#define pSSVEP0_PATTERN_7 (unsigned short volatile*) SSVEP0_PATTERN_7
///**
// * SSVEP0 frequency control word, frequency = MMR_value / 10, 0.1Hz resolution
// */
//#define SSVEP0_FCW (0x20210100)
//#define pSSVEP0_FCW (unsigned short volatile*) SSVEP0_FCW
//
///**
// * SSVEP0 brightness register, from 1 to 4999, dimmest to brightest
// */
//#define SSVEP0_BR (0x20210200)
//#define pSSVEP0_BR (unsigned short volatile*) SSVEP0_BR
//
///**
// * SSVEP0 pattern length register, from 1 to 256
// */
//#define SSVEP0_PATTERN_LEN (0x20210300)
//#define pSSVEP0_PATTERN_LEN (unsigned short volatile*) SSVEP0_PATTERN_LEN
//
///********************************************************************************/
///***************************SSVEP_1 MMR******************************************/
///********************************************************************************/
//
///**
// * SSVEP1 pattern register, 0 to 7 stands for 8*16 bits pattern, LSB first
// */
//#define SSVEP1_PATTERN_0 (0x20211000)
//#define pSSVEP1_PATTERN_0 (unsigned short volatile*) SSVEP1_PATTERN_0
//
//#define SSVEP1_PATTERN_1 (0x20211010)
//#define pSSVEP1_PATTERN_1 (unsigned short volatile*) SSVEP1_PATTERN_1
//
//#define SSVEP1_PATTERN_2 (0x20211020)
//#define pSSVEP1_PATTERN_2 (unsigned short volatile*) SSVEP1_PATTERN_2
//
//#define SSVEP1_PATTERN_3 (0x20211030)
//#define pSSVEP1_PATTERN_3 (unsigned short volatile*) SSVEP1_PATTERN_3
//
//#define SSVEP1_PATTERN_4 (0x20211040)
//#define pSSVEP1_PATTERN_4 (unsigned short volatile*) SSVEP1_PATTERN_4
//
//#define SSVEP1_PATTERN_5 (0x20211050)
//#define pSSVEP1_PATTERN_5 (unsigned short volatile*) SSVEP1_PATTERN_5
//
//#define SSVEP1_PATTERN_6 (0x20211060)
//#define pSSVEP1_PATTERN_6 (unsigned short volatile*) SSVEP1_PATTERN_6
//
//#define SSVEP1_PATTERN_7 (0x20211070)
//#define pSSVEP1_PATTERN_7 (unsigned short volatile*) SSVEP1_PATTERN_7
///**
// * SSVEP1 frequency control word, frequency = MMR_value / 10, 0.1Hz resolution
// */
//#define SSVEP1_FCW (0x20211100)
//#define pSSVEP1_FCW (unsigned short volatile*) SSVEP1_FCW
///**
// * SSVEP1 brightness register, from 1 to 4999, dimmest to brightnest
// */
//#define SSVEP1_BR (0x20211200)
//#define pSSVEP1_BR (unsigned short volatile*) SSVEP1_BR
//
///**
// * SSVEP1 pattern length register, from 1 to 256
// */
//#define SSVEP1_PATTERN_LEN (0x20211300)
//#define pSSVEP1_PATTERN_LEN (unsigned short volatile*) SSVEP1_PATTERN_LEN
//
///********************************************************************************/
///***************************SSVEP_2 MMR******************************************/
///********************************************************************************/
///**
// * SSVEP2 pattern register, 0 to 7 stands for 8*16 bits pattern, LSB first
// */
//#define SSVEP2_PATTERN_0 (0x20212000)
//#define pSSVEP2_PATTERN_0 (unsigned short volatile*) SSVEP2_PATTERN_0
//
//#define SSVEP2_PATTERN_1 (0x20212010)
//#define pSSVEP2_PATTERN_1 (unsigned short volatile*) SSVEP2_PATTERN_1
//
//#define SSVEP2_PATTERN_2 (0x20212020)
//#define pSSVEP2_PATTERN_2 (unsigned short volatile*) SSVEP2_PATTERN_2
//
//#define SSVEP2_PATTERN_3 (0x20212030)
//#define pSSVEP2_PATTERN_3 (unsigned short volatile*) SSVEP2_PATTERN_3
//
//#define SSVEP2_PATTERN_4 (0x20212040)
//#define pSSVEP2_PATTERN_4 (unsigned short volatile*) SSVEP2_PATTERN_4
//
//#define SSVEP2_PATTERN_5 (0x20212050)
//#define pSSVEP2_PATTERN_5 (unsigned short volatile*) SSVEP2_PATTERN_5
//
//#define SSVEP2_PATTERN_6 (0x20212060)
//#define pSSVEP2_PATTERN_6 (unsigned short volatile*) SSVEP2_PATTERN_6
//
//#define SSVEP2_PATTERN_7 (0x20212070)
//#define pSSVEP2_PATTERN_7 (unsigned short volatile*) SSVEP2_PATTERN_7
///**
// * SSVEP2 frequency control word, frequency = MMR_value / 10, 0.1Hz resolution
// */
//#define SSVEP2_FCW (0x20212100)
//#define pSSVEP2_FCW (unsigned short volatile*) SSVEP2_FCW
///**
// * SSVEP2 brightness register, from 1 to 4999, dimmest to brightnest
// */
//#define SSVEP2_BR (0x20212200)
//#define pSSVEP2_BR (unsigned short volatile*) SSVEP2_BR
//
///**
// * SSVEP2 pattern length register, from 1 to 256
// */
//#define SSVEP2_PATTERN_LEN (0x20212300)
//#define pSSVEP2_PATTERN_LEN (unsigned short volatile*) SSVEP2_PATTERN_LEN
//
///********************************************************************************/
///***************************SSVEP_3 MMR******************************************/
///********************************************************************************/
///**
// * SSVEP3 pattern register, 0 to 7 stands for 8*16 bits pattern, LSB first
// */
//#define SSVEP3_PATTERN_0 (0x20213000)
//#define pSSVEP3_PATTERN_0 (unsigned short volatile*) SSVEP3_PATTERN_0
//
//#define SSVEP3_PATTERN_1 (0x20213010)
//#define pSSVEP3_PATTERN_1 (unsigned short volatile*) SSVEP3_PATTERN_1
//
//#define SSVEP3_PATTERN_2 (0x20213020)
//#define pSSVEP3_PATTERN_2 (unsigned short volatile*) SSVEP3_PATTERN_2
//
//#define SSVEP3_PATTERN_3 (0x20213030)
//#define pSSVEP3_PATTERN_3 (unsigned short volatile*) SSVEP3_PATTERN_3
//
//#define SSVEP3_PATTERN_4 (0x20213040)
//#define pSSVEP3_PATTERN_4 (unsigned short volatile*) SSVEP3_PATTERN_4
//
//#define SSVEP3_PATTERN_5 (0x20213050)
//#define pSSVEP3_PATTERN_5 (unsigned short volatile*) SSVEP3_PATTERN_5
//
//#define SSVEP3_PATTERN_6 (0x20213060)
//#define pSSVEP3_PATTERN_6 (unsigned short volatile*) SSVEP3_PATTERN_6
//
//#define SSVEP3_PATTERN_7 (0x20213070)
//#define pSSVEP3_PATTERN_7 (unsigned short volatile*) SSVEP3_PATTERN_7
///**
// * SSVEP3 frequency control word, frequency = MMR_value / 10, 0.1Hz resolution
// */
//#define SSVEP3_FCW (0x20213100)
//#define pSSVEP3_FCW (unsigned short volatile*) SSVEP3_FCW
///**
// * SSVEP3 brightness register, from 1 to 4999, dimmest to brightnest
// */
//#define SSVEP3_BR (0x20213200)
//#define pSSVEP3_BR (unsigned short volatile*) SSVEP3_BR
//
///**
// * SSVEP3 pattern length register, from 1 to 256
// */
//#define SSVEP3_PATTERN_LEN (0x20213300)
//#define pSSVEP3_PATTERN_LEN (unsigned short volatile*) SSVEP3_PATTERN_LEN


/**
 * Function, to set ssevp pattern
 * 4 arguments congifure for 4 SSVEP arrays individually
 * each should be an unsigned short volatile pointer to an 8 elements array
 */
void ssevp_set_pattern(unsigned short volatile *ssvep_base[],
                        unsigned char ssvepNum,
                        unsigned short pattern_len,
                        unsigned short ssvep_pattern[][SSVEP_PATTERN_LEN]);

/**
 * Function, to set ssevp frequency
 * 4 arguments congifure for 4 SSVEP arrays individually
 * each should be an unsigned short volatile pointer, frequency = argument_value, 0.1Hz resolution, from 0.1Hz to 409.6Hz
 */
void ssvep_set_fcw(unsigned short volatile *ssvep_base[],
                    unsigned char ssvepNum,
                    float ssvep_fcw[]);
/**
 * Function, to set ssevp brightness
 * 4 arguments congifure for 4 SSVEP arrays individually
 * each should be an unsigned short volatile pointer, value valid from 0 to 1000
 */
void ssvep_set_br(unsigned short volatile *ssvep_base[],
                    unsigned char ssvepNum,
                    unsigned short ssvep_br[]);
/**
 * Function, to initialize SSVEP configuration
 */
void ssvep_init();
/**
 * Function, to initialize GPIO
 */
void gpio_init();
/**
 * Function, interrupt service routine for SSVEP
 */
void  ssvep_isr ();

#endif
