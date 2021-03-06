#include "IO.h"

int readFileList(char *pPath) {

	int ret = 0;

	do {

		if(NULL == pPath) {
			ret = -1;
			break;
		}
		
		DIR *pDir = NULL;
		/*
			struct dirent
			{
			   long d_ino; //索引节点号
			   off_t d_off; //在目录文件中的偏移
			   unsigned short d_reclen; //文件名长
			   unsigned char d_type; //文件类型
			   char d_name [NAME_MAX+1]; //文件名,最长255字符
			}
		*/
	    struct dirent *ptr;
	    char path[PATH_LEN] = {0};

	    if ((pDir=opendir(pPath)) == NULL) { //打开文件夹
	        perror("Open dir error");
	        ret = -1;
			break;
	    }

	    while ((ptr=readdir(pDir)) != NULL) { //读取文件夹

	        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0) { //忽略文件夹中的.和..
				continue;
			}
	        else if(ptr->d_type == FILE_TYPE) { //文件
				 printf("d_name:%s/%s\n",pPath,ptr->d_name);
			}
	        else if(ptr->d_type == LINK_TYPE) { //链接文件
				printf("d_name:%s/%s\n",pPath,ptr->d_name);
			}	            
	        else if(ptr->d_type == DIR_TYPE) { //目录
				memset(path,'\0',sizeof(path));
	            strncpy(path,pPath,strlen(pPath));
	            strcat(path,"/");
	            strcat(path,ptr->d_name);
	            readFileList(path);
			}
	    }
		
		closedir(pDir);
		pDir = NULL;
		
	}while(0);

	return ret;
}

bool isFileExist(const char * filePath) {

	if(NULL == filePath) {		
		return false;
	}

	if(0 == access(filePath,F_OK)) {
		return true;
	}

	return false;
}

bool isDirExist(const char * dirPath) {

	if(NULL == dirPath) {		
		return false;
	}

	if(NULL == opendir(dirPath)) {
		return false;
	}

	return true;
}

