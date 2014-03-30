/******************************************************************************
 * @file: queueHandler.c
 *
 * @brief:
 *  - The role of this module is to draw points to the LCD
 *  - from the queue
 *
 * @author:  Robin Yohannan
 * @created: 3/21/14
 *
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "picotk.h"
#include <startup.h>
#include <tll6527_core_timer.h>
#include <constant.h>
#include "queue_point.h"
#include "point.h"

/******************************************************************************
                                DEFINITIONS
*******************************************************************************/
#define LCD_FRAMEHEIGHT 320
#define LCD_FRAMEWIDTH 240

/******************************************************************************
                                  GLOBALS
*******************************************************************************/
point_t * currPoint;
isrDisp_t isrDisp;
fb_t frameBuffer;
int xScale=1, yScale=1, zScale=1;

picotk_Color * pixelFrame[LCD_FRAMEHEIGHT][LCD_FRAMEWIDTH];

queue_point_t drawPointQueue;

/******************************************************************************
                            FUNCTION PROTOTYPES
*******************************************************************************/
//void queueHandler_init();
//void queueHandler_clear();
//void queueHandler_draw();
//void queueHandler_pushPoint(point_t *);
//int queueHandler_XPointToPixel(point_t *);
//int queueHandler_YPointToPixel(point_t *);
//picotk_Color queueHandler_ZPointToColor(point_t *);
//int queueHandler_isPointAdded(queue_t *, point_t *);

/******************************************************************************
                              IMPLEMENTATION
*******************************************************************************/

/******************************************************************************
                              PRIVATE FUNCTIONS
*******************************************************************************/
void line_Draw(fb_t *state,int xcorr1,int ycorr1,int xcorr2,int ycorr2,
		int red,int green,int blue){

	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	int i, j;

	float lhseq = 1.0;
	float slope = 1.0;
	float rhseq = 1.0;

	if (xcorr1 <= xcorr2) {
		x1 = xcorr1;
		x2 = xcorr2;
	}
	else {
		x1 = xcorr2;
		x2 = xcorr1;
	}

	if (ycorr1 <= ycorr2) {
		y1 = ycorr1;
		y2 = ycorr2;
	}
	else {
		y1 = ycorr2;
		y1 = ycorr1;
	}

	if (xcorr2 != xcorr1) {
		slope = ((ycorr2 - ycorr1) * 1.0) / ((xcorr2 - xcorr1) * 1.0);
	}
	else {
		slope = 0.0;
	}
	for (i = y1; i <= y2; i++) {
		for (j = x1; j <= x2; j++) {

			lhseq = (i - ycorr1) * 1.0;
			rhseq = slope * ((j - xcorr1) * 1.0);
			if ( lhseq == rhseq) {
				fb_pixelPut(state,red,green,blue,j,i);
			}
			if (x1 == x2) {
				fb_pixelPut(state,red,green,blue,j,i);
			}
		}
	}
}

/**
 * @name queueHandler_init()
 *
 * Initializes the ISR dispatcher +
 * the picotk library, and clears the LCD screen
 *
 * @param void
 * @return void
 */
void queueHandler_init() {
  /* Commented out blackfin setup code
     because this should be done in the
     top-level*/

	int status = isrDisp_init(&isrDisp);
	if ( PASS != status ) {
		printf("Failed to initialize the Interrupt dispatcher\n");
		exit(0);
	}
	fb_Init(&frameBuffer,0,LQ035Q1DH02,RGB565, &isrDisp);
	coreTimer_init();
	queue_init(&drawPointQueue);
}

/**
 * @name setXRange()
 *
 * Sets the X-axis point-to-LCD scale factor
 *
 * @param xNum - number of possible x-axis integers
 * @return void
 */
void setXRange(int xNum) {
  xScale = LCD_FRAMEHEIGHT / xNum;
}

/**
 * @name setYRange()
 *
 * Sets the Y-axis point-to-LCD scale factor
 *
 * @param yNum - number of possible y-axis integers
 * @return void
 */
void setYRange(int yNum) {
  yScale = LCD_FRAMEWIDTH / yNum;
}

/**
 * @name setZRange()
 *
 * Sets the Z-axis point-to-LCD color scale factor
 *
 * @param zNum - number of possible z-axis integers
 * @return void
 */
void setZRange(int zNum) {
  zScale = 255/zNum;
}

/**
 * @name queueHandler_clear()
 *
 * Clears the LCD screen
 *
 * @param void
 * @return void
 */
void queueHandler_clear() {
  fb_Release(&frameBuffer);
  fb_Init(&frameBuffer,0,LQ035Q1DH02,RGB565, &isrDisp);
  queue_clear(&drawPointQueue);
}

/**
 * @name queueHandler_ZPointToColor()
 *
 * Return LCD pixel color using zScale factor
 *
 * @param point - Address to Point object
 * @return picotk_Color - picotk color struct
 */
picotk_Color queueHandler_ZPointToColor(point_t * point) {
  picotk_Color color;
  color.red = (point->z_pos) * zScale;
  color.green = (point->z_pos) * zScale;
  color.blue = (point->z_pos) * zScale;
  return color;
}
/**
 * @name queueHandler_XPointToPixel()
 *
 * Return LCD pixel row using xScale factor
 *
 * @param point - Address to Point object
 * @return int - LCD pixel row
 */
int queueHandler_XPointToPixel(point_t * point) {
  return (point->x_pos) * xScale;
}

/**
 * @name queueHandler_YPointToPixel()
 *
 * Return LCD pixel column using yScale factor
 *
 * @param point - Address to Point object
 * @return int - LCD pixel column
 */
int queueHandler_YPointToPixel(point_t * point) {
  return (point->y_pos) * yScale;
}

int queueHandler_isPointAdded(queue_point_t * q, point_t * pt) {
  point_t * itr = q->firstElement;
  while (itr != NULL) {
    if (itr->x_pos == pt->x_pos &&
	itr->y_pos == pt->y_pos &&
	itr->z_pos == pt->z_pos) {
      return 1;
    }
    itr = itr->nextPoint;
  }
  return 0;
}

/**
 * @name queueHandler_draw()
 *
 * Draw internal Point object on LCD
 *
 * @param void
 * @return void
 */
void queueHandler_draw() {
  picotk_Color tempClr;
  point_t * iter = drawPointQueue.firstElement;
  int i = 0;
  while (iter != NULL) {
	  tempClr = queueHandler_ZPointToColor(iter);
	  if(i == 0){
		  fb_pixelPut(&frameBuffer,tempClr.red,tempClr.green,tempClr.blue,
		  			  queueHandler_YPointToPixel(iter),
		  			  queueHandler_XPointToPixel(iter));
		  i++;
	  }else{
		  line_Draw(&frameBuffer,queueHandler_XPointToPixel(iter->prevPoint),
				  queueHandler_YPointToPixel(iter->prevPoint),
				  queueHandler_XPointToPixel(iter),
				  queueHandler_YPointToPixel(iter),
				  tempClr.red,tempClr.green,tempClr.blue);
	  }
    iter = iter->nextPoint;
    fb_imageShow(&frameBuffer);
  }
}

/**
 * @name queueHandler_pushPoint()
 *
 * Update internal Point object ptr
 *
 * @param pt - Address to Point object
 * @return void
 */
void queueHandler_pushPoint(point_t * pt) {
  queue_addPoint(&drawPointQueue, pt->x_pos, pt->y_pos, pt->z_pos);
}
