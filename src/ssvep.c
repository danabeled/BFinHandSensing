/*
 * ssvep.c
 *
 *  Created on: Apr 22, 2013
 *      Author: Shen Feng
 *      Last change: $Id$
 */
#include <stdio.h>
#include <ssvep.h>
#include "dab_config.h"
#include "tll_config.h"


static unsigned short ssvep_pattern[SSVEP_MAX][SSVEP_PATTERN_LEN];
static unsigned short volatile *ssvep_base[SSVEP_MAX];

static float ssvep_fcw[SSVEP_MAX];

static unsigned short ssvep_br[SSVEP_MAX];
/**
 * Function, to set ssevp pattern
 * 4 arguments congifure for 4 SSVEP arrays individually
 * each should be an unsigned short pointer to an 8 elements array
 */
void ssevp_set_pattern(unsigned short volatile *ssvep_base[],
                        unsigned char ssvepNum,
                        unsigned short pattern_len,
                        unsigned short ssvep_pattern[][SSVEP_PATTERN_LEN]){

    *pSSVEP_OEN = 0;        // disable ssvep output
    // offset is aligned with bytes
    *(unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_LEN)= pattern_len;
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_0))= ssvep_pattern[ssvepNum][0x0];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_1))= ssvep_pattern[ssvepNum][0x1];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_2))= ssvep_pattern[ssvepNum][0x2];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_3))= ssvep_pattern[ssvepNum][0x3];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_4))= ssvep_pattern[ssvepNum][0x4];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_5))= ssvep_pattern[ssvepNum][0x5];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_6))= ssvep_pattern[ssvepNum][0x6];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_7))= ssvep_pattern[ssvepNum][0x7];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_8))= ssvep_pattern[ssvepNum][0x8];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_9))= ssvep_pattern[ssvepNum][0x9];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_a))= ssvep_pattern[ssvepNum][0xa];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_b))= ssvep_pattern[ssvepNum][0xb];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_c))= ssvep_pattern[ssvepNum][0xc];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_d))= ssvep_pattern[ssvepNum][0xd];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_e))= ssvep_pattern[ssvepNum][0xe];
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_PATTERN_f))= ssvep_pattern[ssvepNum][0xf];

    *pSSVEP_OEN = 1;        // enable/disable ssvep output, write 1 to enable, 0 to disable
}

/**
 * Function, to set ssevp frequency
 * each frequency should be an unsigned short pointer, frequency = ssvep_fcw[i], 0.1Hz resolution, from 0.1Hz to 409.6Hz
 */
void ssvep_set_fcw(unsigned short volatile *ssvep_base[],
                    unsigned char ssvepNum,
                    float ssvep_fcw[]){
    *pSSVEP_OEN = 0;        // disable ssvep output
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_FCW))= (unsigned short)(10*ssvep_fcw[ssvepNum]);
    *pSSVEP_OEN = 1;        // enable ssvep output
}
/**
 * Function, to set ssevp brightness
 * each brightness should be an unsigned short pointer, value valid from 0 to 1000
 */
void ssvep_set_br(unsigned short volatile *ssvep_base[],
                    unsigned char ssvepNum,
                    unsigned short ssvep_br[]){
    *pSSVEP_OEN = 0;        // disable ssvep output
    *((unsigned short volatile*)((void*)ssvep_base[ssvepNum] + OFFSET_BR))= 5*ssvep_br[ssvepNum];
    *pSSVEP_OEN = 1;        // enable ssvep output
}
/**
 * Function, to initialize SSVEP configuration
 */
void ssvep_init(){

    int i;

    // initialized ssvep base address arrary
    ssvep_base[0] = pSSVEP0_BASE;
    ssvep_base[1] = pSSVEP1_BASE;
    ssvep_base[2] = pSSVEP2_BASE;
    ssvep_base[3] = pSSVEP3_BASE;

    // dummy ssvep pattern, frequency and brightness generation
    for(i=0; i < SSVEP_MAX; i++){

        ssvep_pattern[i][0x0] = 0x5050;
        ssvep_pattern[i][0x1] = 0x5150;
        ssvep_pattern[i][0x2] = 0x5250;
        ssvep_pattern[i][0x3] = 0x5350;
        ssvep_pattern[i][0x4] = 0x5450;
        ssvep_pattern[i][0x5] = 0x5550;
        ssvep_pattern[i][0x6] = 0x5650;
        ssvep_pattern[i][0x7] = 0x5750;
        ssvep_pattern[i][0x8] = 0x5850;
        ssvep_pattern[i][0x9] = 0x5950;
        ssvep_pattern[i][0xa] = 0x5a50;
        ssvep_pattern[i][0xb] = 0x5b50;
        ssvep_pattern[i][0xc] = 0x5c50;
        ssvep_pattern[i][0xd] = 0x5d50;
        ssvep_pattern[i][0xe] = 0x5e50;
        ssvep_pattern[i][0xf] = 0x5f50;

        ssvep_fcw[i] = 400.0;
        ssvep_br[i] = 300;
    }
    // set ssvep pattern for each ssvep channel
    for(i=0; i < SSVEP_MAX; i++){
        ssevp_set_pattern(ssvep_base, i, 256, ssvep_pattern);
    }
    // set ssvep frequency for each ssvep channel
    for(i=0; i < SSVEP_MAX; i++){
        ssvep_set_fcw(ssvep_base, i, ssvep_fcw);
    }
    // set ssvep brightness for each ssvep channel
    for(i=0; i < SSVEP_MAX; i++){
        ssvep_set_br(ssvep_base, i, ssvep_br);
    }
    // enable 4 ssvep channels, write 1 for enable
    *pSSVEP_MASK = 15;
//    asm("nop;");
//    asm("ssync;");
}
/**
 * Function, to initialize GPIO
 */
void gpio_init(){
    *pGPIO_EN = 0xff;//enable as GPIO
    *pGPIO_OE = 0xf0;//configure i/o
    *pGPIO_IN_INTE = 0x0f;//interrupt enable
    *pGPIO_OUT = 0xf0;     // '1100'
}

/**
 * Function, interrupt service routine for SSVEP
 */
void  ssvep_isr ()
{
    int j=100;
    while (j>0){
        asm("nop;");
        j--;
    }

//        printf("gpio_val : %x\n", *pGPIO_IN);
//        printf("Pend : %x\n", *pEXINTPEND);
        *pPORTHIO_CLEAR |= PH11;
        *pEXINTPEND = 31;

        asm("ssync;");

}


