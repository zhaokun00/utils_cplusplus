#include <stdio.h>
#include <stdlib.h>

#include "DataTag.h"

int addDataTag(short *pData, unsigned int len,unsigned short tag) {

    int ret =  0;
    int i = 0;
    
    do {
		//判断所传的short类型数据的长度必须大于DATA_TAG_LEN
        if ((pData == NULL) || (len < DATA_TAG_LEN)) {
            ret = -1;
            break;
        }

		//数据的前16个short类型中,每一个short类型的数据的最低位存放数据DATA_TAG_START=0x7FFE
        for (i = DATA_TAG_BIT_START; i < DATA_TAG_BIT_END; i ++) {
            pData[i] = ((pData[i] & DATA_TAG_BIT_MASK) | ((DATA_TAG_START & (DATA_TAG_ONE_BIT << i)) >> i));
        }

		//紧接着头信息开始的16个short类型数据,每一个short类型的数据的最低位存放number的最低位
        for (i = DATA_TAG_BIT_START; i < DATA_TAG_BIT_END; i ++) {
            pData[i + DATA_OFFSET] = ((pData[i + DATA_OFFSET] & DATA_TAG_BIT_MASK) | ((tag & (DATA_TAG_ONE_BIT << i)) >> i));
        }
         
    }
    while (0);
    
    return ret;
}

int getDataTag(short *pData, unsigned int len, unsigned short *pTag) {

    int ret =  0;
    int i = 0;
	int j = 0;
    unsigned short dataTag = 0;
    int frames = 0;
	
    do 
    {
        if ((pData == NULL) || (len < DATA_TAG_LEN) || (pTag == NULL)) {
            ret = -1;
            break;
        }

		frames = len / DATA_OFFSET;
		
		for(j = 0; j < frames; j++) { //每16个short类型循环检查一遍
		
			dataTag = 0;
			for(i = DATA_TAG_BIT_START; i < DATA_TAG_BIT_END; i++) {
				dataTag |= ((pData[i] & DATA_TAG_ONE_BIT) << i);
			}

			if (dataTag == DATA_TAG_START) {
				pTag[0] = 0;
				if(j != frames - 1) {
					for (i = DATA_TAG_BIT_START; i < DATA_TAG_BIT_END; i++) {
					pTag[0] |= ((pData[i + DATA_OFFSET] & DATA_TAG_ONE_BIT) << i);
					}
				}
				
				break;
			}

			if(j != frames - 1) {
				pData = pData + DATA_OFFSET;
			}
		}            
    }
    while (0);
    
    return ret;    
}
