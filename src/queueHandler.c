/******************************************************************************
 *@file: queueHandler.c
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

#include <stdio.h>
#include <stdlib.h>
#include <picotk.h>
#include <queue.h>
#include <point.h>
#include <startup.h>
#include <tll6527_core_timer.h>
#include <constant.h>

#define LCD_FRAMEHIGHT 320
#define LCD_FRAMEWIDTH 240

//point_t * nextPoint;
//point_t * prevPoint;
point_t * currPoint;

int xScale=1, yScale=1, zScale=1;

isrDisp_t isrDisp;
fb_t frameBuffer;

/******************************************************************************
                            FUNCTION PROTOTYPES
*******************************************************************************/
void queueHandler_init();
void queueHandler_clear();
void queueHandler_draw();
void queueHandler_pushPoint();
int queueHandler_XPointToPixel(point_t * point);
int queueHandler_YPointToPixel(point_t * point);
picotk_Color queueHandler_ZPointToColor(point_t *point);

/******************************************************************************
                              IMPLEMENTATION
*******************************************************************************/

void queueHandler_init() {
  int ret = 0;
  ret = blackfin_setup();
  
  if (ret) {
    printf("\r\n Blackfin Setup Failed \r\n");
    exit(0);
  }

  isrDisp_init(&isrDisp);
  picotk_Init(&isrDisp);
  RTC_waitForInit();

  picotk_DestSet(LCD_FB);
}

void setXRange(int xNum) {
  xScale = LCD_FRAMEHEIGHT / xNum;
}

void setYRange(int yNum) {
  yScale = LCD_FRAMEWIDTH / yNum;
}
void setZRange(int yNum) {
  zScale = 255/zNum;
}

void queueHandler_clear() {
  picotk_Init(&isrDisp);
}

void queueHandler_draw() {
  picotk_DrawPoint(&queueHandler_ZPointToColor(currPoint),
		   queueHandler_XPointToPixel(currPoint),
		   queueHandler_YPointToPixel(currPoint));
  picotk_ShowNB();
  picotk_ReadyToDraw();
  free(currPoint);
}

void queueHandler_pushPoint(point_t * pt) {
  currPoint = pt;
}

int queueHandler_XPointToPixel(point_t * point) {
  return (point->x) * xScale;
}

int queueHandler_YPointToPixel(point_t * point) {
  return (point->y) * yScale;
}

picotk_Color queueHandler_ZPointToColor(point_t * point) {
  picotk_Color color;
  color.red = (point->z) * zScale;
  color.green = (point->z) * zScale;
  color.blue = (point->z) * zScale;
  return color;
}
