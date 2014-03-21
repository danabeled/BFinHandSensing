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
#include <picotk.h>
#include <queue.h>
#include <point.h>
#include <startup.h>
#include <tll6527_core_timer.h>
#include <constant.h>

point_t * nextPoint;
point_t * prevPoint;
point_t currPoint;

int ptXMax, ptYMax, ptZMax;

isrDisp_t isrDisp;
fb_t frameBuffer;

/******************************************************************************
                            FUNCTION PROTOTYPES
*******************************************************************************/
void queueHandler_init();
void queueHandler_clear();
void queueHandler_draw();
void queueHandler_unreceivePoint();
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

void queueHandler_clear() {
  picotk_Init(&isrDisp);
}

void queueHandler_draw() {
  while ((currPoint = queue_getPoint()) != ERROR) {
    
    picotk_DrawPoint(&queueHandler_ZPointToColor(&currPoint),
		     queueHandler_XPointToPixel(&currPoint),
		     queueHandler_YPointToPixel(&currPoint));
  }
  picotk_ShowNB();
  picotk_ReadyToDraw();
}

void queueHandler_unreceivePoint() {
}

int queueHandler_XPointToPixel(point_t * point) {
  int x = point.x;
  int xPix;
  /* X to Pixel Mapping */
  
  return xPix;
}

int queueHandler_YPointToPixel(point_t * point) {
  int y = point.y;
  int yPix;
  /* Y to Pixel Mapping */
  
  return yPix;
}

picotk_Color queueHandler_ZPointToColor(point_t * point) {
  int z = point.z;
  picotk_Color color;
  /* Z to Color Mapping */
  
  return color;
}
