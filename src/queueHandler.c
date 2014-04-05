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
		exit(-1);
	}
	picotk_Init(&isrDisp);
	RTC_waitForInit();

	//the foreground buffer
	picotk_DestSet(LCD_FB);

	coreTimer_init();
	currPoint = NULL;
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

void drawEmptyPoint(){
	picotk_Color color;
	color.red = 0;
	color.green = 0;
	color.blue = 0;
	picotk_DrawPoint(&color,0,0);
	picotk_Show();
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

  // Reinitialize the Interrupt Dispatcher
  int status = isrDisp_init(&isrDisp);
  if ( PASS != status ) {
    printf("Failed to initialize the Interrupt dispatcher\n");
    exit(-1);
  }
  queue_clear(&drawPointQueue);
  drawEmptyPoint();
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
  point_t * iter = drawPointQueue.firstElement;
  picotk_Color tempClr;

  while(iter != NULL){
	  tempClr = queueHandler_ZPointToColor(iter);
	  if(drawPointQueue.queueSize == 1){
		  picotk_DrawPoint(&tempClr,
				  queueHandler_YPointToPixel(iter),
				  queueHandler_XPointToPixel(iter));
	  }else{
		  picotk_DrawLine(&tempClr,
				  queueHandler_XPointToPixel(iter->prevPoint),
				  queueHandler_YPointToPixel(iter->prevPoint),
				  queueHandler_XPointToPixel(iter),
				  queueHandler_YPointToPixel(iter));
	  }
	  iter = iter -> nextPoint;
  }
  picotk_Show();
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

void queueHandler_display(){
	point_t * iter = drawPointQueue.firstElement;
	int i = 0;
	while(iter != NULL){
		point_print(iter);
		iter = iter -> nextPoint;
	}
	printf("---------------\r\n");
}
