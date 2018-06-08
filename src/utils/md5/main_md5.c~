#include "md5.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include <fstream>

#define READ_DATA_SIZE	1024
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)

/*
总结:
	windows下写回车换行:程序中写入\r\n即可
	linux下写回车换行:程序中写入\n即可

	对于一些windows下的智能软件来说,可以识别linux系统的回车换行(\n),而对于windows下的记事本来说只能识别\r\n

windows系统中有如下等价关系:
用enter换行<=====>程序写入\r\n<=====>真正向文件中写\r\n(0x0d 0x0a)<=====>程序真正读取的是\r\n

linux系统中的等价关系:
用enter换行<=====>程序写入\n<=====>真正向文件中写\n(0x0a)<=====>程序真正读取的是\n

举例:假设在linux有一个txt文件,那么它在文件中的换行标志是\n,现在把txt文件拷贝到windows上,那windows对txt文件说:别跟我整什么\n,我只认\r\n,
如果这个txt文件里面有\r\n,那我就认为是换行符,否则我不认你。如此一来,windows压根就找不到txt中的\r\n,所以,对于windows而言压根就没有发现
txt文件中有任何换行符,所以我们从windows上看到的txt文件中显示在一行里面

同理,假设在windows上有一个txt文件,那么它在文件中的换行符是\r\n,现在拷贝到linux下,那好啊,linux说:别的我不管,我遇到文件中的\n我就认为是换行,至于其他的,我只认为是正常的字符,
如此一来,\r就被当成文件的正常部分,也就是说,linux下的程序不仅仅会读取到\n,也会读取到他前面的\r

hexdump命令一般用来查看二进制文件的十六进制编码,但实际上它能查看任何文件,而不仅限于二进制文件

文件内容:12

实例:
hexdump -C 文件名称

00000000  31 32 0a                                          |12.|
00000003

用命令hexdump 文件名
输出结果为:

0000000 3231 000a                              
0000003

上述结果是由于存储的大小端问题

用命令查看md5值:
md5sum 文件名

*/

//C接口
int Compute_string_md5(unsigned char *dest_str, unsigned int dest_len, char *md5_str)
{
	int i;
	unsigned char md5_value[MD5_SIZE];
	MD5_CTX md5;

	MD5Init(&md5);

	MD5Update(&md5, dest_str, dest_len);

	MD5Final(&md5, md5_value);

	for(i = 0; i < MD5_SIZE; i++)
	{
		snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
	}

	return 0;
}

int Compute_file_md5(const char *file_path, char *md5_str)
{
	int i;
	int fd;
	int ret;
	unsigned char data[READ_DATA_SIZE];
	unsigned char md5_value[MD5_SIZE];
	MD5_CTX md5;

	fd = open(file_path, O_RDONLY);
	if (-1 == fd)
	{
		perror("open");
		return -1;
	}

	MD5Init(&md5);

	int sum  = 0;
	while (1)
	{
		ret = read(fd, data, READ_DATA_SIZE);
		if (-1 == ret)
		{
			perror("read");
			close(fd);
			return -1;
		}

		MD5Update(&md5, data, ret);

		if (0 == ret || ret < READ_DATA_SIZE)
		{
			break;
		}
	}

	close(fd);

	MD5Final(&md5, md5_value);

	for(i = 0; i < MD5_SIZE; i++)
	{
		snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
	}

	return 0;
}

//C++接口
std::string computeStringMd5(std::string str, unsigned int destLen) {

	int i = 0;
	unsigned char md5Value[MD5_SIZE];
	char md5Str[MD5_STR_LEN + 1];
	MD5_CTX md5;

	MD5Init(&md5);

	MD5Update(&md5, (unsigned char *)str.c_str(), destLen);

	MD5Final(&md5, md5Value);

	for(i = 0; i < MD5_SIZE; i++) {
		snprintf(md5Str + i*2, 2+1, "%02x", md5Value[i]);
	}

	return md5Str;
}

std::string computeFileMd5(const char *filePath)
{
	int ret;
	unsigned char data[READ_DATA_SIZE];
	unsigned char md5_value[MD5_SIZE];
	char md5_str[MD5_STR_LEN + 1];
	MD5_CTX md5;

	std::ifstream t(filePath);
	
	if(!t.is_open()) { //在这里判断打开文件是否成功
		std::cout << "打开文件" << filePath << "失败" << std::endl;
		return "";
	}

	MD5Init(&md5);

	while(!t.eof()) { //用于判断文件是否读到结尾,还可以用t.gcout() > 0来进行判断

		t.read((char *)data,sizeof(data));

		if(t.gcount() > 0) { //t.gcout-用于获取实际读取到的字节数
			MD5Update(&md5, data, t.gcount());
		}
		else {
			break;
		}
	}

	t.close();

	MD5Final(&md5, md5_value);

	for(int i = 0; i < MD5_SIZE; i++)
	{
		snprintf(md5_str + i*2, sizeof(md5_str), "%02x", md5_value[i]);
	}

	return md5_str;
}

/************** main test **************/
int main(int argc, char *argv[])
{
#if 1
	int ret;
	const char *file_path = "/home/zhaokun/utils/utils_cplusplus/src/utils/md5/md5.c";
	char md5_str[MD5_STR_LEN + 1];
	const char *test_str = "gchinaran@gmail.com";
#if 1
	// test file md5
	ret = Compute_file_md5(file_path, md5_str);
	if (0 == ret)
	{
		printf("[file - %s] md5 value:\n", file_path);
		printf("%s\n", md5_str);
	}
#endif

#if 0
	// test string md5 
	Compute_string_md5((unsigned char *)test_str, strlen(test_str), md5_str);
	printf("%s\n", md5_str);

	std::string s = computeStringMd5(test_str, strlen(test_str));

	std::cout << s << std::endl;
#endif
	std::string s = computeFileMd5(file_path);

	std::cout << s << std::endl;
#endif

#if 0

	std::string str = "123";
	unsigned char *t = (unsigned char *)str.c_str();

	std::cout << t << std::endl;
#endif
	
#if 0
	std::string str = "123";

	std::string s1 = "abc";

	str = s1.append(str);

	std::cout << str << std::endl;

	std::cout << str.substr(2,0) << std::endl;
#endif

	return 0;
}



