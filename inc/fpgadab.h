#ifndef _FPGADAB_H_
#define _FPGADAB_H_

#include "queue.h"
#include "bufferPool.h"
#include "isrDisp.h"

typedef struct {
  queue_t queue; /* queue for received buffers */
  chunk_t *pPending; /* pointer to pending chunk */
  bufferPool_t *pBuffP; /* pointer to buffer pool */
} fpgadab_t;

int fpgadab_init(fpgadab_t *pThis, bufferPool_t *pBuffP, isrDisp_t *pIsrDisp);
int fpgadab_start(fpgadab_t *pThis);
void fpgadab_fifoISRr(void *pThisArgs);
int fpgadab_getNbNc(fpgadab_t *pThis, chunk_t **ppChunk);
int setSampleRate(unsigned int fsample, unsigned int os);
void DAQ_inHouse_init();

#define FPGADAB_QUEUE_DEPTH (7)

#endif
