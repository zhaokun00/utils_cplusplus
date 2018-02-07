#ifndef DATA_TAG_H____
#define DATA_TAG_H____

#define DATA_TAG_START        0x7FFE

#define DATA_TAG_BIT_START    0
#define DATA_TAG_BIT_END      16
#define DATA_OFFSET    		  16
#define DATA_TAG_LEN		  32
#define DATA_TAG_BIT_MASK     0xFFFE
#define DATA_TAG_ONE_BIT      0x0001

/** 
* @brief add data's tag
* @param pData:data's pointer,len:data's len,tag:data's tag
* @return 0:success,-1:failed
*/	
int addDataTag(short *pData, unsigned int len,unsigned short tag);

/** 
* @brief get data's tag
* @param pData:data's pointer,len:data's len,tag:data's tag pointer
* @return 0:success,-1:failed
*/	
int getDataTag(short *pData, unsigned int len, unsigned short *tag);

#endif

