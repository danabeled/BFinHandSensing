/******************************************************************************
 *@file: queueHandler.h
 *
 *@brief:
 *  - The role of this module is to draw points to the LCD
 *  - from the queue
 *
 * Target:   TLL6527v1-0
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author:  Robin Yohannan
 * @created: 3/21/14
 *
 *******************************************************************************/

#ifndef QUEUE_HANDLER_H
#define QUEUE_HANDLER_H


void queueHandler_init();

void queueHandler_clear();

void queueHandler_draw();

void queueHandler_pushPoint();

void setXRange(int xNum);

void setYRange(int yNum);

void setZRange(int zNum);

#endif
