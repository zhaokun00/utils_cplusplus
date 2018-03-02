#ifndef C_LIBCURL_H____
#define C_LIBCURL_H____

	#include <curl/curl.h>
	#include <string.h>
	#include <string>
	#include <unistd.h> 
	using std::string;

	enum LibcurlMethod {
	    Lm_None = 0,  
	    Lm_Download,  
	    Lm_Post,  
	    Lm_Get,  
	};

	typedef void (*T)(const char *pData,size_t nSize);
	typedef void (*E)(void *pParam, double total, double now);
	
	class CLibcurl {

	public:  
	    CLibcurl();  
	    ~CLibcurl();  

	    bool setPort(unsigned int port);                                             //设置连接端口号  
	    bool setTimeout(unsigned int nSecond);                                       //设置执行超时（秒）  
	    bool setConnectTimeout(unsigned int nSecond);                                //设置连接超时（秒）  
	    bool setUserAgent(const char * pAgent);                              	     //设置用户代理  
	    bool setResumeFrom(long pos);                                      		 //设置断点续传起始位置  
	    bool setResumeFromLarge(long long pos);                            		 //设置断点续传起始位置，针对大文件  
	    bool addHeader(const char * pKey, const char * pValue);                    			     //添加自定义头  
	    void clearHeaderList();                                             		 //清理HTTP列表头  
	    bool setCookie(const char * pCookie);                                 		     //设置HTTP请求cookie  
	    bool setCookieFile(const char * pFilePath);                              			 //设置HTTP请求cookie文件    
	    bool downloadToFile(const char * pUrl, const char *pFile,void * param,E callBack);                            //下载文件到磁盘  
	    bool post(const char * pUrl, const char* pData,T callBack);   //Post字符串或者二进制数据  
	    bool get(const char * pUrl,T callBack);                                                 //Get请求  
	  
	protected:  
	    static size_t   writeCallback(void* pBuffer, size_t nSize, size_t nmemb, void* pParam);  
//	    static size_t   HeaderCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam);  
	    static int      progressCallback(void *pParam, double total, double now, double ultotal, double ulnow);  
	  
	private:  
	    CURL			*m_pCurl; //curl的句柄  
	    unsigned int    m_port;   //server的端口
	    
	    CURLcode 		m_curlCode; //执行libcurl返回码 
	    LibcurlMethod 	m_method;  
	    curl_slist 		*m_curlList;
		T 				m_response; //用户重写响应函数
		FILE*  			m_File; //
	    void    		*m_downLoadParam; //下载要传递的参数  
		E    			m_downCallBack;  //用户重写下载进度函数
	};

#endif

