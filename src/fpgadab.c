/*****************************************************************************
 *@file DAB_interrupt.c
 *
 *@brief
 *  - Interrup service routine. 
 *  - Read data back from FPGA according to the channel number.
 *  - In this implement the PortF0 is defined as the interrupt signal.
 *	- The mapping with schematic is PF0/PPID0/DROPRI/ND_D0A.
 *	- The mapping with FPGA design is PF0_PPID0_DROPRI.
 *
 * Target:   TLL6527v1-1      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 *@author    Ran Hao, ECE, Northeastern University (Based on the "gpio_interrupt.c")
 *@date      01/16/2011
 *
 * LastChange:
 *  $ EX_INTERRUPT_HANDLER changed by RHao, 20110116 $
 *
 *******************************************************************************/

#include <stdio.h>
#include "startup.h"
#include "tll_config.h"
#include <sys/exception.h>
//#include "dab_interrupt.h"
#include "dab_config.h"
#include "tll6527_core_timer.h"
//#include "config.h"
#include "fpgadab.h"
#include "dab_config.h"
#include "bufferPool.h"
#include "chunk.h"
#include "ssvep.h"

/******************** Global Data Instantiation***********************/
/**
 * @param count		  counts the number of times it enters into the ISR
 * @param FIFOSIZE  store the storage status (how many data in FIFO) 
 */
 
//extern unsigned long count;

/**************** Private Define **********************************************/
/**
 * @def INPUT_BITS   (0xFFFE)
 * @brief Configure GPIO port F (lower 1 input) for input
  */
#define INPUT_BITS     (0xFFFE)

/**
 * @def INPUT_EN   (0x0001)
 * @brief Configure GPIO port F (lower 1 input) input enable
  */
#define INPUT_EN     (0x0001)

static unsigned int pressed = 0;
static unsigned short INT = 0;

/**************** Method Implementation *****************************/

/** 
 * Interrupt service routine for FIFO_full signal (from PORTF0), store 
 * data sampled from ADC 
 * Read sampled data in channels back. If enough space for one sample
 * per channel is not available, put chunk into queue, waiting to be
 * shipped away and acquire a new queue. Then put 8 channel data in Chunk.
 * Clean PIC pending reg. Clean PH11 interrupt port.
 *
 * Parameters:	*pThisArgs : pointer to fpgadab_t object
 *
 * @return void
 */
void fpgadab_fifoISR(void *pThisArgs) {

    fpgadab_t *pThis = (fpgadab_t *) pThisArgs;
    unsigned short isrSource = 0;

    pressed++; // debug counter for counting how often button was pressed

    unsigned short j;
    unsigned short status = 0;

    if(pThis == NULL) {
      printf("[FPGADAB]: failed to get (NULL struct.\n");
    }

    // check the isr source in PIC.
    isrSource = *pEXINTPEND;
    // TODO: your functionality.


    // clear the pending in PIC.
    *pEXINTPEND = isrSource; // Clear the pending reg in FPGA
    INT              = *pPORTHIO & PH11;   // read current state
	*pPORTHIO_CLEAR     = INT;      // clear all inputs -> clear interrupt

//	ssvep_isr();

    asm("nop;");                       // add two nop/ssync cylces to ensure int clr arrives
    asm("ssync;");
    asm("nop;");
    asm("ssync;");
}



/**
 * set the sampling frequency and counter for interval between CONVST and nCS
 * so that the FPGA ADC_signal_gen module can generate correct sclk, nCS for ADC
 * check Table 9 in AD7606 manual for tradeoff between oversampling rate and maximum throughput
 *
 * Tcycle = 1/fsample
 * Tcycle = Tconvst + Tconv + Tcs
 * Tconvst = 30ns; Tconv = 10ns*convCnt; Tcs = 128*Tsclk;
 * Tsclk maximum 20MHz
 *
 * time unit: ns
 *
 * Parameters: sampling frequency below 200ksps; oversampling rate in [0,2,4,8,16,32,64]
 *
 * @return void;
 */
int setSampleRate(unsigned int fsample, unsigned int os){
	// time unit: ns
	unsigned int Tcycle = 0;//    // test len
	unsigned int Tconvst = 30;
	unsigned int Tconv = os * CONVRATIO;
	unsigned int Tcs = 0;
	unsigned int sclkCnt = 0;
	unsigned short convCnt = 0;

	Tcycle = 1000000000/fsample;
	// Tcycle = Tconvst + Tbusy + Tcs
	Tcs = (Tcycle - Tconvst - Tconv);
	// Tcs = 128*Tsclk; fsclk = 100MHz / (2*sclkCnt)
	sclkCnt = Tcs/128/10/2;
	// Tconv = 10ns*convCnt
	convCnt = Tconv / 10;

	// Vdrive = 5V; Maximum fsclk = 20MHz = 100MHz / (2*sclkCnt)
	if (sclkCnt > 2 && sclkCnt < 65536){
		*pADCCONG = convCnt;	// conversion period = T(system clk) * convCnt
		*pSCLKCONG = (unsigned short)sclkCnt;
//        *pADCCONG = 38400;		// FSAMPLE = 512
//        *pSCLKCONG = 612;
		return PASS;
	} else {
		printf("[FPGADAB]: fsclk fails to exceed 20MHz\n");
		return FAIL;
	}
	asm("nop;");
	asm("ssync;");
	return 0;
}

/** 
 * FPGA reset routine. Setup setup sclk cycle, select whether go through filter, and mask all the 
 * interrupt at the beginning. Then reset the FIFO space and setup the FIFO size.
 *
 * Parameters:
 *
 * @return void
 */
void DAQ_inHouse_init(){
  /* Initialization: Registers in FPGA
     -- FIFOStatus	: [1, 255]
     -- pFIFOCONG	: setup FIFO size;
     -- pSCLKCONG	: setup sclk frequency;
     -- pExIntMask 	: disable(mask) interrupt;
     -- pFIRSEL		: filter selection
  */

//  *pFIRSEL	= FIREN; // filter plug in
  *pEXINTMASK = 0x00FF; // mask all interrupt
  coreTimer_delay(100);
  *pFIFORST  = FIFORST; //Reset FIFO in FPGA

  setSampleRate(FSAMPLE,OS64);


  *pFIFO_FULL_THR = FIFOSIZE; // Setup FIFO configuration
  *pEXINTEDGE = 0x00FF; // set 1 for edge sensitive
  *pEXINTPOL  = 0x00FF; // High level

  *pEXINTPEND = 0x00FF;
  *pEXINTMASK = 0x00F0; //Enable the interrupt

  gpio_init();
  ssvep_init();

  *pADC_START = 0;

  asm("nop;");
  asm("ssync;");

}


 
/** 
 *
 * Initialization of PORTFIO. This PORT is used as GPIO.
 * The output and input direction can be set using the MACROS
 *
 * Parameters:
 *
 * @return void
 */
int fpgadab_init(fpgadab_t *pThis, bufferPool_t *pBuffP, isrDisp_t *pIsrDisp)
{

  if(pThis == NULL || pBuffP == NULL || pIsrDisp == NULL) {
    printf("[FPGADAB]: Failed init\n");
    return FAIL;
  }

  pThis-> pBuffP = pBuffP;
  pThis-> pPending = NULL;
  queue_init(&pThis->queue, FPGADAB_QUEUE_DEPTH);

  // acquire a new chunk for first triggered ISR
  if(bufferPool_acquire(pThis->pBuffP, &(pThis->pPending)) == FAIL) {
    printf("[FPGADAB]: failed to acquire chunk\n");
  }

  // Reset FIFO
  DAQ_inHouse_init();

  // register isr
  isrDisp_registerCallback(pIsrDisp,ISR_PORT_H_INTERRUPT_A , fpgadab_fifoISR, pThis);

  *pPORTH_FER     &= ~PH11;              // set 0 select GPIO functionality for PH11
  *pPORTHIO_DIR   &= ~PH11;    // set 0 for input PH11
  *pPORTHIO_INEN  |= PH11;     // 1 enable input for all push / switches
  *pPORTHIO_CLEAR |= PH11;         // clear inputs -> clear any pending ints
  *pPORTHIO_EDGE  |= PH11;         // select 1 to edge sensitivity
  *pPORTHIO_POLAR &= ~PH11;         // set 0 for rising edge
  *pPORTHIO_BOTH  &= ~PH11;              // set 0 for not both edges
  *pPORTHIO_MASKA |= PH11;         // map to Interrupt A output of PORTH

  //*pSIC_IMASK0    |= 0x20000000;     // Enable PORTH Int A at the System Int Controller

  return PASS;

}

int fpgadab_getNbNc(fpgadab_t *pThis, chunk_t **ppChunk) {
  // check if queue is empty
  if(queue_is_empty(&pThis->queue)) {
    return FAIL;
  } else {
    queue_get(&pThis->queue, (void**)ppChunk);
    return PASS;
  }
}

