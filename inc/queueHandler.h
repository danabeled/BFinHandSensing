/******************************************************************************
 * @file: queueHandler.h
 *
 * @brief:
 *  - The role of this module is to draw points to the LCD
 *  - from the queue
 *
 * @author:  Robin Yohannan
 * @created: 3/21/14
 *
 *******************************************************************************/

#ifndef QUEUE_HANDLER_H
#define QUEUE_HANDLER_H

/**
 * @name queueHandler_init()
 *
 * Initializes the ISR dispatcher +
 * the picotk library, and clears the LCD screen
 *
 * @param void
 * @return void
 */
void queueHandler_init();

/**
 * @name queueHandler_clear()
 *
 * Clears the LCD screen
 *
 * @param void
 * @return void
 */
void queueHandler_clear();

/**
 * @name queueHandler_draw()
 *
 * Draw internal Point object on LCD
 *
 * @param void
 * @return void
 */
void queueHandler_draw();

/**
 * @name queueHandler_pushPoint()
 *
 * Update internal Point object ptr
 *
 * @param pt - Address to Point object
 * @return void
 */
void queueHandler_pushPoint();

/**
 * @name setXRange()
 *
 * Sets the X-axis point-to-LCD scale factor
 *
 * @param xNum - number of possible x-axis integers
 * @return void
 */
void setXRange(int xNum);

/**
 * @name setYRange()
 *
 * Sets the Y-axis point-to-LCD scale factor
 *
 * @param yNum - number of possible y-axis integers
 * @return void
 */
void setYRange(int yNum);

/**
 * @name setZRange()
 *
 * Sets the Z-axis point-to-LCD color scale factor
 *
 * @param zNum - number of possible z-axis integers
 * @return void
 */
void setZRange(int zNum);

#endif
