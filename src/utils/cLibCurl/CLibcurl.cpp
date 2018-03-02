#include "CLibCurl.h"

CLibcurl::CLibcurl()  
    : m_pCurl(NULL)  
    , m_port(80)
    , m_curlCode(CURLE_OK)
    , m_curlList(NULL) 
    , m_response(NULL)
    , m_File(NULL)
    , m_downLoadParam(NULL)
    , m_downCallBack(NULL) {
    
    m_pCurl = curl_easy_init();
	curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, true);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, writeCallback); //设置回调函数  
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this); //将本身对象传递进去
} 

CLibcurl::~CLibcurl(void) {

	clearHeaderList();	
	curl_easy_cleanup(m_pCurl);
}

bool CLibcurl::setPort(unsigned int port) {

    m_port = port;  
    m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_PORT, m_port); //设置端口  
    return CURLE_OK == m_curlCode;
}  

bool CLibcurl::setTimeout(unsigned int nSecond) {

    if(nSecond<0)  
        return false;
	
    m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, nSecond); //设置执行超时时间
    return CURLE_OK == m_curlCode;  
}

bool CLibcurl::setConnectTimeout(unsigned int nSecond) {

	if(nSecond<0)  
        return false;

	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, nSecond); //设置连接超时时间
    return CURLE_OK == m_curlCode;
}

bool CLibcurl::setUserAgent(const char * pAgent) {

    if (NULL == pAgent)  
        return false; 
	
    int nLen = strlen(pAgent);  
    if (nLen == 0)  
        return false;
	
    m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_USERAGENT, pAgent); //设置用户代理 
    return CURLE_OK == m_curlCode;  
}

bool CLibcurl::setResumeFrom(long pos) {  

    if (pos<0 )  
        return false;
	
    m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM, pos);  
    return CURLE_OK == m_curlCode;  
}  
  
bool CLibcurl::setResumeFromLarge(long long pos) {  

    if (pos<0 )  
        return false;
	
    m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM_LARGE, pos);  
    return CURLE_OK == m_curlCode;  
}

bool CLibcurl::addHeader(const char * pKey, const char *pValue ) {

	if((NULL == pKey) || (NULL == pValue)) {

		return false;
	}
	
    int keyLen = strlen(pKey);
	int valueLen = strlen(pValue);

	if((keyLen <= 0) || (valueLen <= 0)) {
		return false;
	}
	
    string strHeader = pKey;
	string value = pValue;
    strHeader.append(":");  
    strHeader.append(value);
	
    m_curlList = curl_slist_append(m_curlList, strHeader.c_str());
    m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_curlList);
	
    return CURLE_OK == m_curlCode;  
} 

void CLibcurl::clearHeaderList() { 

    if (m_curlList) {  
        curl_slist_free_all(m_curlList);  
        m_curlList = NULL;  
    }  
}

bool CLibcurl::setCookie(const char *pCookie) {

    if(NULL == pCookie)
		return false;
	
    m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_COOKIE, pCookie);  
    return CURLE_OK == m_curlCode;  
} 

bool CLibcurl::setCookieFile(const char *pFilePath) { 

    if(NULL == pFilePath)
		return false;
	
    m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE, pFilePath);  
    return CURLE_OK == m_curlCode;  
} 

bool CLibcurl::downloadToFile(const char * pUrl, const char *pFile,void * param,E callBack) {  

	m_downLoadParam = param;
	m_downCallBack = callBack;
	
    curl_easy_setopt(m_pCurl, CURLOPT_URL, pUrl);  
	unlink(pFile);

	m_File = fopen(pFile,"wb"); //创建文件
	
    if (NULL == m_File) {
        return false;  
    }  
    curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, 0);  
    curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, progressCallback);  
    curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSDATA, this);  
    m_method = Lm_Download;  
     
    m_curlCode = curl_easy_perform(m_pCurl);  
    return CURLE_OK == m_curlCode;  
}  

bool CLibcurl::post(const char * pUrl, const char* pData,T callBack) {  

    if((NULL == pUrl) || (NULL == pData))
		return false;

	m_response = callBack;
    curl_easy_setopt(m_pCurl, CURLOPT_POST, 1);  
    curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, pData);  
    curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, strlen(pData));  
    curl_easy_setopt(m_pCurl, CURLOPT_URL, pUrl);

    m_method = Lm_Post;
  
    m_curlCode = curl_easy_perform(m_pCurl);  
    return CURLE_OK == m_curlCode;  
}

bool CLibcurl::get(const char * pUrl,T callBack) {  

	if(NULL == pUrl)
		return false;

	m_response = callBack;
    curl_easy_setopt(m_pCurl, CURLOPT_HTTPGET, 1);  
    curl_easy_setopt(m_pCurl, CURLOPT_URL, pUrl);  
    curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);//支持重定向  
    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);  
    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
	
    m_method = Lm_Get;  
     
    m_curlCode = curl_easy_perform(m_pCurl);  
    return CURLE_OK == m_curlCode;  
} 

size_t CLibcurl::writeCallback( void* pBuffer, size_t nSize, size_t nmemb, void* pParam) {  

    //把下载到的数据以追加的方式写入文件(一定要有a，否则前面写入的内容就会被覆盖了)  
    CLibcurl* pThis = (CLibcurl*)pParam;
	 
    switch(pThis->m_method) { 
	    case Lm_Download:
            if(NULL == pThis->m_File)  
                return 0; 
            fwrite((char *)pBuffer,nSize,nmemb,pThis->m_File); //写文件操作
	        break;  
	    case Lm_Post://Post数据  
	    case Lm_Get://Get数据
	    	if(pThis->m_response) {
				pThis->m_response((const char *)pBuffer,nSize*nmemb);
			}
	        break;  
	    case Lm_None://未定义  
	        break;  
    }
	
    return nSize*nmemb;  
} 

int CLibcurl::progressCallback(void *pParam, double total, double now, double ultotal, double ulnow) {

    CLibcurl* pThis = (CLibcurl*)pParam;  
    if (pThis->m_downCallBack) { 
        pThis->m_downCallBack(pThis->m_downLoadParam, total, now);  
    }  
    return 0;  
}


