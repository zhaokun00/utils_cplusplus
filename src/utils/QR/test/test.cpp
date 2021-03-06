/*
	1.下载源码:
	https://github.com/rsky/qrcode
	2.编译源码
	2.1在qrcode/libqr/目录下新建build目录,执行cmake ..命令
	2.2将编译好的libqr.so  libqr.so.1  libqr.so.1.0.0复制到需要的目录里面 cp -dR lib* 目录	

	备注:libqr库依赖于zlib库
	
	编译:
	g++ test.cpp -o test -L. -lqr

	导出当前目录:
	export LD_LIBRARY_PATH=./

	运行:
	./test

*/
#include <stdio.h>
#include <fstream>
#include "qr.h"

using namespace std;
/*
libqr使用起来非常简单,只需要包含qr.h头文件,编译的时候把库文件引进去即可,使用libqr生成二维码分成4个步骤:
1.初始化二维码句柄
2.添加数据
3.调用finalize
4.生成图片
*/
int main()
{
        int errcode = QR_ERR_NONE;
		/*
			函数原型:
			QR_API QRCode *qrInit(int version, int mode, int eclevel, int masktype, int *errcode)
			qrInit的5个参数分别是版本,mode,纠错等级和掩码，使用过程中使用除了QR_EM_8BIT以外的mode会生成不正常的二维码，暂时不知道原因。
		*/

		//步骤1:初始化二维码句柄
        QRCode* p = qrInit(10, QR_EM_8BIT, 2, -1, &errcode);

        if (p == NULL) {
		    printf("error\n");
		    return -1;
        }

        string strData = "http://www.baidu.com";

		/*
			函数原型:
			int qrAddData(QRCode *qr, const qr_byte_t *source, int size);
		*/
		//步骤2:添加数据
        qrAddData(p, (const qr_byte_t* )strData.data(), strData.length());

		/*
			int qrFinalize(QRCode *qr);
		*/
        //步骤3:注意需要调用qrFinalize函数
        if (!qrFinalize(p)) {
            printf("finalize error\n");
            return -1;
        }

        int size = 0;

		/*
			函数原型:			
			qrSymbolToPNG(QRCode *qr, int sep, int mag, int *size)
		*/
        //两个5分别表示：像素之间的距离和二维码图片的放大倍数，范围都是1-16

		//步骤4:生成图片
        qr_byte_t * buffer = qrSymbolToPNG(p, 5, 5, &size);
        if (buffer == NULL) {
            printf("error %s", qrGetErrorInfo(p));
            return -1;
        }

	
		//步骤5:将数据写到文件中
        ofstream f("a.png");
        f.write((const char *)buffer, size);
        f.close();

        return 0;
}
