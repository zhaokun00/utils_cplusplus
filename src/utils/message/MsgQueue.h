#ifndef MSG_QUEUE_H____
#define MSG_QUEUE_H____

#include <stdio.h>
#include <sys/types.h>  
#include <sys/msg.h>  
#include <unistd.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>

#define MSG_PATH_NAME "/data/10.100.37.17/home/zhaok/test/utils_cplusplus/src/main.cpp"
#define MSG_ID 'm'

class MsgQueue       {

	public:
		static int Create();
		static MsgQueue * getMsgQue();
		static int Destory();
		//传入的为消息的指针,将来在消息队列中会复制一个消息,即使源消息释放掉,消息队列中的消息也不会消失,除非取走
	    int send(const void *msg, unsigned int nBytes, int flag);
		//传入的为消息队列中的消息指针,会将消息队列中的消息,复制一个一份给目的消息,将消息从消息队列中取走后,消息队列中的消息将会消失
	    int reveive(void *msg, unsigned int nbytes, long type, int flag);
		int getMsgId();
		static const int BLOCK = 0;
		static const int NOBLOCK = IPC_NOWAIT;
	private:
		MsgQueue();
		//把复制构造函数和=操作符设为私有,防止被复制
		MsgQueue(const MsgQueue &);
		MsgQueue & operator=(const MsgQueue &);
		static int createQueueId();
		static MsgQueue * sMsgQue;
		static key_t sMsgKey;  //创建消息队列的key
	    static int sMsgQueueID;//消息队列的ID  
};
#endif


/*
	linux关于消息队列中消息的大小的限制

	在消息队列中，例如函数msgsnd(int msqid, const void* msgp, size_t msgsz, int msgflg);
	这个函数调用的时候，msgsz最大只能为8192，也就是2的16次方。可以看出这里的msgsz大小限制在一个short型。
	超过这个大小就会出错——invalid argument。并不是其他人所说的只要msgsz是mtext的大小就不会出错，
	如果sizeof(mtype)+sizeof(mtext)<=8192时，msgsz为sizeof(mtype)+sizeof(mtext)大小也没有关系。

	当然msgsz这个大小也不是不可以改变，如果要变，就去内核代码里面关于实现消息队列的程序中把这个限制改变一下就好。（
	我没试过，应该可以），一般是在内核源码中的ipc文件夹中会有mqueue.c这个c语言程序文件，里面会定义DFLT_MSGSIZEMAX为8192，
	这应该就是为什么msgsz最大为8192的原因，如果要改，可以改掉，然后重新编译内核。

	/proc/sys/kernel/msgmax　　　 单个消息占用的最大字节数　　　　  缺省值为 8192
	/proc/sys/kernel/msgmnb   　　消息队列能容纳的消息数　     				  缺省值为 16384
	/proc/sys/kernel/msgmni  　　 系统中能容纳消息队列的个数　　　缺省值为　16
	cat /proc/sys/kernel/shmmax
	可通过下面的方式进行设置
	echo 819200 > /proc/sys/kernel/msgmax
	echo 1638400 > /proc/sys/kernel/msgmnb        
	echo 1600 > /proc/sys/kernel/msgmni 
	 
	cd /proc/sys/kernel ; cat msgmax;cat msgmnb; cat msgmni; 
	设置完毕，要删除原来的消息队列，再建立才能生效。
	ipcrm -q 需要删除队列的名称

	 // 运行记录
	echo 1638400 > /proc/sys/kernel/msgmnb
	------ Message Queues --------
	key        msqid      owner      perms      used-bytes   messages   
	0x00008114 196608     root       666        1638201      5936       
	0x00008115 32769      root       666        0            0          
	0x00001899 262146     root       777        0            0          
	0x00008110 98307      root       666        120          5          
	0x00008112 131076     root       666        0            0          
	0x00008113 163845     root       666        0            0
	 
	[root@localhost kernel]# cat msgmax;cat msgmnb; cat msgmni;
	819200
	6000000
	1600
	 
	------ Message Queues --------
	key        msqid      owner      perms      used-bytes   messages   
	0x00008114 294912     root       666        5999859      21638      
	0x00008115 32769      root       666        0            0          
	0x00001899 262146     root       777        0            0          
	0x00008110 98307      root       666        504          21         
	0x00008112 131076     root       666        0            0          
	0x00008113 163845     root       666        0            0 
	共享内存的参数查看和设置
	/proc/sys/kernel/shmall 
	/proc/sys/kernel/shmmax 
	/proc/sys/kernel/shmmni
*/

