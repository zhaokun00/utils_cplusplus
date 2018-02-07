 
#include <stdint.h>
#include <assert.h>
#include "RingBuf.h"

static inline void	_getRingBufferAvailBytes(RingBufferT *pRingBuf, uint32_t *read, uint32_t *write) {

	uint32_t read_loc, write_loc;

	read_loc = pRingBuf->readPos;
	write_loc = pRingBuf->writePos;

	*write = BYTES_AVAIL_TO_WRITE(read_loc, write_loc, pRingBuf->size);
	*read = pRingBuf->size - *write;
}

static inline uint32_t _getNextWriteLocation(RingBufferT *pRingBuf) {

	uint32_t next = pRingBuf->writePos;

	assert(next < pRingBuf->size);

	return next;
}

static inline void _setNextWriteLocation(RingBufferT *pRingBuf, uint32_t next) {

	pRingBuf->writePos = next;
}

static inline uint32_t _getNextReadLocation(RingBufferT *pRingBuf) {

	uint32_t next = pRingBuf->readPos;

	assert(next < pRingBuf->size);

	return next;
}

static inline void _setNextReadLocation(RingBufferT *pRingBuf, uint32_t next) {

	pRingBuf->readPos = next;
}

static inline void* _getRingBuffer(RingBufferT *pRingBuf) {

	return (void *)pRingBuf->pBuf;
}

static inline uint32_t _getRingBufferSize(RingBufferT *pRingBuf) {

	return pRingBuf->size;
}

static uint32_t _copyToRingBuffer(RingBufferT *pRingBuf, void* Src, uint32_t SrcLen, uint32_t startPos) {

	void* ringBuffer =_getRingBuffer(pRingBuf);
	uint32_t ringBufferSize = _getRingBufferSize(pRingBuf);
	uint32_t fragLen;

	if (SrcLen > (ringBufferSize - startPos)) {
	
		fragLen = ringBufferSize - startPos;
		memcpy((char *)ringBuffer + startPos, Src, fragLen);
		memcpy(ringBuffer, (char *)Src + fragLen, SrcLen - fragLen);
	}
	else {
	
		memcpy((char *)ringBuffer + startPos, Src, SrcLen);
	}

	return ((startPos + SrcLen) % ringBufferSize);
}

static uint32_t _copyFromRingBuffer(RingBufferT *pRingBuf, void* Dest, uint32_t DestLen, uint32_t startPos) {

	void* ringBuffer = _getRingBuffer(pRingBuf);
	uint32_t ringBufferSize = _getRingBufferSize(pRingBuf);
	uint32_t fragLen;

	if (DestLen > ringBufferSize - startPos) {
	
		fragLen = ringBufferSize - startPos;

		memcpy(Dest, (char *)ringBuffer + startPos, fragLen);
		memcpy((char *)Dest + fragLen, ringBuffer, DestLen - fragLen);
	}
	else {
	
		memcpy(Dest, (char *)ringBuffer + startPos, DestLen);
	}

	return ((startPos + DestLen) % ringBufferSize);
}

/*
 * Internal functions end
 */
int initRingBuf(RingBufferT *pRingBuf, uint32_t bufSize, uint32_t queueSize) {

	int ret = ERR_NOERROR;

	do
	{
		if (pRingBuf == NULL) {
			printf("invalid input params\n");
			ret = -ERR_NORMALERROR;
			break;
		}
		
		pRingBuf->cx = CriticalSectionPosix::Create();
		
        if (pRingBuf->cx == NULL) {
			printf("invalid input params\n");
			ret = -ERR_NORMALERROR;
			break;	            
        }

		pRingBuf->pBuf = (char *)malloc(bufSize);
		
		if (pRingBuf->pBuf == NULL) {
			printf("invalid input params\n");
			ret = -ERR_NORMALERROR;
			break;			
		}

		pRingBuf->size = bufSize;
		pRingBuf->readPos = 0;
		pRingBuf->writePos = 0;
		pRingBuf->queueSize = queueSize;
		pRingBuf->queueCnt = pRingBuf->size / pRingBuf->queueSize;
	}
	while (0);

	if (ret != ERR_NOERROR) {
		releaseRingBuf(pRingBuf);
	}

	return ret;
}


void releaseRingBuf(RingBufferT *pRingBuf) {

	do
	{
		if (pRingBuf == NULL) {
			printf("invalid input params\n");
			break;
		}

		if  (pRingBuf->pBuf != NULL) {
			free(pRingBuf->pBuf);
			pRingBuf->pBuf = NULL;
		}
        
        if (pRingBuf->cx != NULL) {
            delete pRingBuf->cx;
            pRingBuf->cx = NULL;
        } 
	}
	while (0);
}


int readRingBuf(RingBufferT *pRingBuf, char *pData, uint32_t len) {

	int ret = ERR_NOERROR;
	uint32_t bytesAvailToWrite;
	uint32_t bytesAvailToRead;
	uint32_t startPos;
	uint32_t nextReadLocation=0;  
   
	do
	{
		if ((pRingBuf == NULL) || (pData == NULL)) {
		
			printf("invalid input params\n");
			ret = -ERR_NORMALERROR;
			break;
		}

        CriticalSectionScoped critScoped(pRingBuf->cx);
        
		_getRingBufferAvailBytes(pRingBuf, &bytesAvailToRead, &bytesAvailToWrite);

		if (bytesAvailToRead < len)
		{
			printf("not enough data to read\n");
			ret = -ERR_NOTENOUGHDATA;
			break;
		}

		startPos = _getNextReadLocation(pRingBuf);
		nextReadLocation = _copyFromRingBuffer(pRingBuf, pData, len, startPos);
		_setNextReadLocation(pRingBuf, nextReadLocation);	
	}
	while (0);

    // FUNC_L;
    
	return ret;	
}


int writeRingBuf(RingBufferT *pRingBuf, char *pData, uint32_t len) {

	int ret = ERR_NOERROR;
	uint32_t bytesAvailToWrite;
	uint32_t bytesAvailToRead;
	uint32_t startPos;
	uint32_t nextWriteLocation;

	do
	{
		if ((pRingBuf == NULL) || (pData == NULL))
		{
			printf("invalid input params\n");
            ret = -ERR_NORMALERROR;
			break;
		}

        CriticalSectionScoped critScoped(pRingBuf->cx);

		_getRingBufferAvailBytes(pRingBuf, &bytesAvailToRead, &bytesAvailToWrite);	
		
		if (bytesAvailToWrite < len)
		{
			printf("not enough buffer to write\n");
			ret = -ERR_NOTENOUGHDATA;
			break;
		}	
		
		startPos = _getNextWriteLocation(pRingBuf);
		nextWriteLocation = _copyToRingBuffer(pRingBuf, pData, len, startPos);
		_setNextWriteLocation(pRingBuf, nextWriteLocation);
		
		if(pRingBuf->readPos == pRingBuf->writePos) {
			return -1;
		}
	}
	while (0);

	return ret;	
}


uint32_t getWritePosRingBuf(RingBufferT *pRingBuf) {

	return _getNextWriteLocation(pRingBuf);
}


uint32_t getReadPosRingBuf(RingBufferT *pRingBuf) {

	return _getNextReadLocation(pRingBuf);
}

