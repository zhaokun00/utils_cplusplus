/*
 **************************************************************************************
 * Filename   : RingBuf.h
 * Author 	  : zhaokun
 * Description: RingBuf.h
 * Date		  ：2018-01-30
 * Others	  :
 **************************************************************************************
 */

#ifndef RINGBUF_H____
#define RINGBUF_H____

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include "CriticalSectionPosix.h"

#define ERR_NOERROR					0
#define ERR_NORMALERROR				1
#define ERR_NOTENOUGHDATA           10

#define BYTES_AVAIL_TO_WRITE(r, w, z) ((w) >= (r))?((z) - ((w) - (r))):((r) - (w))

typedef struct RingBufferS {

	unsigned int size; //ring buf的大小,单位字节
	char *pBuf; //ring buf的指针

	uint32_t writePos; //写位置
	uint32_t readPos; //读位置

	CriticalSectionPosix *cx; //锁

	uint32_t queueSize; //每一块的大小,单位字节
	uint32_t queueCnt; //块个数,其中size = queueSize * queueCnt

} RingBufferT;

 /*
  * Function: initRingBuf
  * Description: Initialize ring buffer
  * Parameters:
  *	RingBufferT *pRingBuf: Ring buffer object PTR
  *	uint32_t bufSize: Total buffer size of the ring buffer
  * uint32_t queueSize: per queue's size
  * Return:
  *	0 - success
  *	-1 - failure
  */
int initRingBuf(RingBufferT *pRingBuf, uint32_t bufSize, uint32_t queueSize);
 
 /*
  * Function: releaseRingBuf
  * Description: release ring buffer for audio media data
  * Parameters:
  *	RingBufferT *pRingBuf: Ring buffer object PTR
  * Return:
  *	None
  */
void releaseRingBuf(RingBufferT *pRingBuf);

 /*
  * Function: readRingBuf
  * Description: read specified length data from ring buffer.
  * Parameters:
  *	MediaRingBufferT *pRingBuf: Ring buffer object PTR
  *	char *pData: Buffer to receive data
  *	unsigned int len: Length to read
  * Return:
  *	0 - success
  *	-1 - failure
  */

int readRingBuf(RingBufferT *pRingBuf, char *pData, uint32_t len); 

/*
  * Function: writeMediaRingBuf
  * Description: write specified length data ring buffer for audio media data
  * Parameters:
  *	MediaRingBufferT *pRingBuf: Ring buffer object PTR
  *	unsigned int bufSize: Total buffer size of the ring buffer
  * Return:
  *	0 - success
  *	-1 - failure
  * note:丢弃数据将来会丢失整个缓冲区的数据,可以根据应用实际情况将来修改代码来适应
  */

int writeRingBuf(RingBufferT *pRingBuf, char *pData, uint32_t len);

/*
  * Function: getWritePosMediaRingBuf
  * Description: get the current wirte position of ring buffer.
  * Parameters:
  *	MediaRingBufferT *pRingBuf: Ring buffer object PTR
  * Return:
  *	Current write position
  */
uint32_t getWritePosRingBuf(RingBufferT *pRingBuf);

/*
  * Function: getReadPosMediaRingBuf
  * Description: get the current read position of ring buffer.
  * Parameters:
  *	MediaRingBufferT *pRingBuf: Ring buffer object PTR
  * Return:
  *	Current read position
  */

uint32_t getReadPosRingBuf(RingBufferT *pRingBuf);


#endif

