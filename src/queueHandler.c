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
#include <picotk.h>
#include <queue.h>
#include <point.h>
#include <startup.h>
#include <tll6527_core_timer.h>
#include <constant.h>

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

queue_t drawPointQueue;

/******************************************************************************
                            FUNCTION PROTOTYPES
*******************************************************************************/
void queueHandler_init();
void queueHandler_clear();
void queueHandler_draw();
void queueHandler_pushPoint();
int queueHandler_XPointToPixel(point_t *);
int queueHandler_YPointToPixel(point_t *);
picotk_Color queueHandler_ZPointToColor(point_t *);
int queueHandler_isPointAdded(queue_t *, point_t *);

/******************************************************************************
                              IMPLEMENTATION
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
     top-level
  */
  /* int ret = 0; */
  /* ret = blackfin_setup(); */
  
  /* if (ret) { */
  /*   printf("\r\n Blackfin Setup Failed \r\n"); */
  /*   exit(0); */
  /* } */

  isrDisp_init(&isrDisp);
  picotk_Init(&isrDisp);
  RTC_waitForInit();

  picotk_DestSet(LCD_FB);

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
  picotk_Init(&isrDisp);
  queue_clear(&drawPointQueue);
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
  while (iter != NULL) {
    tempClr = queueHandler_ZPointToColor(iter);
    picotk_DrawPoint(&tempClr,
		     queueHandler_XPointToPixel(iter),
		     queueHandler_YPointToPixel(iter));
    iter = iter->nextPoint;
  }
  
  picotk_ShowNB();
  picotk_ReadyToDraw();
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
  if (!queueHandler_isPointAdded(&drawPointQueue, pt)) {
    queue_addPoint(&drawPointQueue, pt->x_pos, pt->y_pos, pt->z_pos);
  }

  free(pt);
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

int queueHandler_isPointAdded(queue_t * q, point_t * pt) {
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
