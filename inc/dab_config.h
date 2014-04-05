/******************************************************************************
 *@file: dab_config.h
 *
 *@brief: 
 *	- Configuration file which has specific macros for this app only
 * 		This Incudes:
 * 		- PIC IO definition for PIC;
 * 		- FIFO read address;
 * 		- Filter selection register;
 * 		- SCLK configuration register (Write);
 * 		- FIFO size status register;
 * 		- FIFO size configuration register;
 * 		- FIFO reset register;
 *
 *
 * Target:   TLL6527Mv1-0
 * Compiler: Visual DSP 5.0 updata 7    Output format: VDSP++ "*.dxe"
 *
 * @author:    Ran Hao, ECE, Northeastern University
 * @created: 20110114
 *
 *******************************************************************************/
#ifndef DAB_CONFIG_H_
#define DAB_CONFIG_H_ 

// FPGA address type
typedef unsigned short volatile *fpga_addr_t;

//Register Interfaces

/************************************************************************
 * Initialization of PIC registers;
 *	 ExIntEdge : EdgeEnable Register '1' = edge triggered interrupt source
 *                                   '0' = level triggered interrupt source 
 *   ExIntPol  : PolarityRegister    '1' = high level / rising edge
 *                                   '0' = low level / falling edge
 *   ExIntMask : MaskRegister        '1' = interrupt masked (disabled) 
 *                                   '0' = interrupt not masked (enabled)    
 *   ExIntPend : PendingRegister     '1' = interrupt pending
 *                                   '0' = no interrupt pending                           
 ***********************************************************************/  
#define EXINTEDGE 0x202ffff8
#define pEXINTEDGE ((unsigned short volatile *)EXINTEDGE) 

#define EXINTPOL 0x202ffffa
#define pEXINTPOL ((unsigned short volatile *)EXINTPOL) 

#define EXINTMASK 0x202ffffc
#define pEXINTMASK ((unsigned short volatile *)EXINTMASK) 

#define EXINTPEND 0x202ffffe
#define pEXINTPEND ((unsigned short volatile *)EXINTPEND) 

/************************************************************************
 *	First Frame Register 
 *	-- Store the first frame of the transimission. It indicates who generate
 *	   the Interrupt.
 *	   UART --> --;
 *	   FIFO --> 0xAAAB;	   
 ***********************************************************************/  
#define INTINFO 0x20280000
#define pINTINFO ((unsigned short volatile *)INTINFO) 

/************************************************************************
 *	FIFO read address:

 ***********************************************************************/  
#define RXFIFO 0x20200000
#define pRXFIFO ((unsigned short volatile *)RXFIFO)

#define TXFIFO 0x20201000
#define pTXFIFO ((unsigned short volatile *)TXFIFO)

#define FIFO_FULL_THR 0x20202000
#define pFIFO_FULL_THR ((unsigned short volatile *)FIFO_FULL_THR)

#define FIFO_STATUS 0x20203000
#define pFIFO_STATUS ((unsigned short volatile *)FIFO_STATUS)

#define ADC_START 0x20205000
#define pADC_START ((unsigned short volatile *)ADC_START)

/************************************************************************
 *	SCLK configuration register (Write):
 *		fsclk = f(system clk)/(N*2)
 ***********************************************************************/  
#define SCLKCONG  0x20200000
#define pSCLKCONG ((unsigned short volatile *)SCLKCONG) /* Interrupt Reset Vector Address Register */

/************************************************************************
 *	Conversion time configuration register (Write):
 *		conversion period = T(system clk) * convCnt
 ***********************************************************************/
#define ADCCONG  0x20204000
#define pADCCONG ((unsigned short volatile *)ADCCONG) /* Interrupt Reset Vector Address Register */
/************************************************************************
 *	FIFO size configuration register (Write):
 *		[1, 255]
 ***********************************************************************/  
#define FIFOCONG  0x20208002
#define pFIFOCONG ((unsigned short volatile *)FIFOCONG) /* Interrupt Reset Vector Address Register */

/************************************************************************
 *	Filter selection register (Write):
 *		[0, 1](volatile unsigned short)
 ***********************************************************************/  
#define FIRSEL  0x20290004
#define pFIRSEL ((unsigned short volatile *)FIRSEL) /* Interrupt Reset Vector Address Register */

/************************************************************************
 *	FIFO size status register (Read):
 ***********************************************************************/  
#define FIFOSTA  0x20208000
#define pFIFOSTA ((unsigned short volatile *)FIFOSTA) /* Interrupt Reset Vector Address Register */

/************************************************************************
 *	FIFO reset (Write):
 *		*pFIFORST = 0x0000; 
 *	Write any value to this address will reset the FIFO array.
 ***********************************************************************/  

#define pFIFORST ((unsigned short volatile *)0x202f0000) /* Interrupt Reset Vector Address Register */


#define ASYNCBANK2_BASE 0x20240000
#define ASYNCBANK3_BASE 0x20300000

#define DATATESTADDR ASYNCBANK2_BASE
#define TESTADDR 0x20280000

#define SPRAM_BASE 	ASYNCBANK2_BASE
//#Bytes of SPRAM instantiated in TLL6527M FPGA
#define SPRAM_SIZE 24576


#define ChannelNum 9
#define FIFOSIZE 64*ChannelNum
#define ADCCLK 100
#define FIREN 1
#define EXINTMASK_SET 0x0001
#define EXINTMASK_CLE 0x0000

// default reset value
#define FIFORST 0x0001

/* default oversampling rate: conversion time in ns
 * default sampling rate
 * check Table 9 in AD7606 manual for tradeoff
 * between oversampling rate and maximum throughput
 */
#define OS0 5000
#define OS2 10000
#define OS4 20000
#define OS8 40000
#define OS16 80000
#define OS32 160000
#define OS64 320000
#define FSAMPLE 512
#define CONVRATIO 1.2


/* SDRAM allocation for the 8 channels' data */
#define SDRAM1addr 0x02000000
#define SDRAM2addr 0x02100000
#define SDRAM3addr 0x02200000
#define SDRAM4addr 0x02300000
#define SDRAM5addr 0x02400000
#define SDRAM6addr 0x02500000
#define SDRAM7addr 0x02600000
#define SDRAM8addr 0x02700000
/* Block storage size is 0x10000 (65536) samples */
#define pSDRAM1addr ((unsigned short volatile *)SDRAM1addr)
#define pSDRAM2addr ((unsigned short volatile *)SDRAM2addr)
#define pSDRAM3addr ((unsigned short volatile *)SDRAM3addr)
#define pSDRAM4addr ((unsigned short volatile *)SDRAM4addr)
#define pSDRAM5addr ((unsigned short volatile *)SDRAM5addr)
#define pSDRAM6addr ((unsigned short volatile *)SDRAM6addr)
#define pSDRAM7addr ((unsigned short volatile *)SDRAM7addr)
#define pSDRAM8addr ((unsigned short volatile *)SDRAM8addr)



#endif
