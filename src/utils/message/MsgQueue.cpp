#include "MsgQueue.h"

key_t MsgQueue::sMsgKey = 0;
int MsgQueue::sMsgQueueID = 0;
MsgQueue * MsgQueue::sMsgQue = new MsgQueue();

MsgQueue::MsgQueue() {};
MsgQueue::MsgQueue(const MsgQueue &) {};
MsgQueue & MsgQueue::operator=(const MsgQueue &) { return *this;};

int MsgQueue::createQueueId() {

	int ret = 0;

	do {
		sMsgQueueID = msgget(sMsgKey, IPC_CREAT|0666);
		
	    if(-1 == sMsgQueueID) {
			printf("createQueueId faild:%s", strerror(errno));  
			ret = -1;
	       	break;
	    }
	} while(0);
	
	return ret;
}

/*
	新创建消息队列,有时候进程非正常退出时,消息队列没有删除,如果里面还有消息,将会对程序的下一次
	运行产生影响,下面的函数可保证是新创建的消息队列
*/
int MsgQueue::Create() {

	int ret = 0;
	
	do {
		
		/*
			函数原型:
			key_t  ftok( const	char * pathname , int	proj_id  );
		
			参数:
			pathname:指定的文件名(该文件必须是存在而且可以访问)
			id:子序号,虽然为int,但是只有8bit被使用
		
			返回值:成功时:返回key_t类型的值,失败返回-1
		*/
		
		sMsgKey = ftok(MSG_PATH_NAME, MSG_ID);
		if(-1 == sMsgKey) {
			printf("MsgQueue create failed\n");
			ret = -1;
			break;
		}

		/*
			函数原型:
			int    msgget ( key_t  key , int  msgflg );
			函数描述：建立消息队列
			msgget()函数的第一个参数是消息队列对象的关键字(key)，函数将它与已有的消息队列对象的关键字进行比较来判断消息队列对象是否已经创建。
			而函数进行的具体操作是由第二个参数，msgflg 控制的。它可以取下面的几个值：
			IPC_CREAT ：如果消息队列对象不存在，则创建之，否则则进行打开操作;
			IPC_EXCL：和IPC_CREAT 一起使用（用”|”连接），如果消息对象不存在则创建之，否则产生一个错误并返回
			
			返回值：
			成功时返回队列ID,失败返回-1，错误原因存于error中
			EEXIST (Queue exists, cannot create)
			EIDRM (Queue is marked for deletion)
			ENOENT (Queue does not exist)
			ENOMEM (Not enough memory to create queue)
			ENOSPC (Maximum queue limit exceeded)
		
		*/
		int msgQueueId = msgget(sMsgKey, IPC_CREAT|IPC_EXCL|0666);

		if(-1 == msgQueueId) {
			createQueueId();
			msgctl(sMsgQueueID, IPC_RMID, NULL); //从内核中移除消息队列
			sMsgQueueID = 0;
			ret = createQueueId();
			break;
		}

		sMsgQueueID = msgQueueId;
		
	} while(0);

	return ret;
}

int MsgQueue::Destory() {

	/*
		函数原型:int msgctl ( int msgqid, int cmd, struct msqid_ds *buf );
		
	*/
	int ret = msgctl(sMsgQueueID, IPC_RMID, NULL);
	
    if(-1 == ret) { 
        printf("Destroy() failed:%s", strerror(errno));  
    }

	if(NULL != sMsgQue) {
		delete sMsgQue;
		sMsgQue = NULL;
	}

	return ret;
}

MsgQueue * MsgQueue::getMsgQue() {

	if(NULL != sMsgQue) {
		return sMsgQue;
	}

	return NULL;
		
}

int MsgQueue::send(const void * msg, unsigned int nBytes, int flag) {

	/*
	函数原型：int  msgsnd ( int msgid ,  struct msgbuf*msgp , int msgsz, int msgflg );
	参数说明：
	传给msgsnd()函数的第一个参数msqid 是消息队列对象的标识符（由msgget()函数得到），第二个参数msgp 指向要发送的消息所在的内存，第三个参数msgsz 是要发送信息的长度（字节数），可以用以下的公式计算：
	msgsz = sizeof(struct mymsgbuf) - sizeof(long);
	第四个参数是控制函数行为的标志，可以取以下的值：
	0，忽略标志位；
	IPC_NOWAIT，如果消息队列已满，消息将不被写入队列，控制权返回调用函数的线
	程。如果不指定这个参数，线程将被阻塞直到消息被可以被写入
	*/
	int ret = msgsnd(sMsgQueueID, msg, nBytes, flag);  
    if(-1 == ret) {
        printf("send msg failed:%s", strerror(errno));   
    }

	return ret;
}

int MsgQueue::reveive(void * msg, unsigned int nBytes, long type, int flag) {

	/*
	函数定义：int	msgrcv( int  msgid , struct   msgbuf*  msgp ,  int msgsz ,	long msgtyp, int msgflg);
	参数：
	函数的前三个参数和msgsnd()函数中对应的参数的含义是相同的。第四个参数mtype
	指定了函数从队列中所取的消息的类型。函数将从队列中搜索类型与之匹配的消息并将
	之返回。不过这里有一个例外。如果mtype 的值是零的话，函数将不做类型检查而自动返	   回队列中的最旧的消息。第五个参数依然是是控制函数行为的标志，取值可以是：
	0,表示忽略；
	IPC_NOWAIT，如果消息队列为空，则返回一个ENOMSG，并将控制权交回调用函数
	的进程。如果不指定这个参数，那么进程将被阻塞直到函数可以从队列中得到符合条件的消息为止。如果一个client 正在等待消息的时候队列被删除，EIDRM 就会被返回。如果	   进程在阻塞等待过程中收到了系统的中断信号，EINTR 就会被返回。
	MSG_NOERROR，如果函数取得的消息长度大于msgsz，将只返回msgsz 长度的信息，
	剩下的部分被丢弃了。如果不指定这个参数，E2BIG 将被返回，而消息则留在队列中不 	被取出。
	当消息从队列内取出后，相应的消息就从队列中删除了。
	msgbuf：结构体，定义如下：
	struct msgbuf
	{
		long	mtype ;  //信息种类
		char   mtest[x];//信息内容	，长度由msgsz指定
	}
	
	msgtyp:  信息类型。 取值如下：
	msgtyp = 0 ，不分类型，直接返回消息队列中的第一项
	msgtyp > 0 ,返回第一项 msgtyp与 msgbuf结构体中的mtype相同的信息
	msgtyp <0 , 返回第一项 mtype小于等于msgtyp绝对值的信息
	
	msgflg:取值如下：
	IPC_NOWAIT ,不阻塞
	IPC_NOERROR ，若信息长度超过参数msgsz，则截断信息而不报错。
	
	返回值：
	成功时返回所获取信息的长度，失败返回-1，错误信息存于error
	Number of bytes copied into message buffer
	-1 on error: errno = E2BIG (Message length is greater than
	msgsz,no MSG_NOERROR)
	EACCES (No read permission)
	EFAULT (Address pointed to by msgp is invalid)
	EIDRM (Queue was removed during retrieval)
	EINTR (Interrupted by arriving signal)
	EINVAL (msgqid invalid, or msgsz less than 0)
	ENOMSG (IPC_NOWAIT asserted, and no message exists in the queue to satisfy the request)
	*/
	int ret = msgrcv(sMsgQueueID,msg, nBytes, type, flag);  
	if( -1 == ret) {
	   printf("receive msg failed:%s", strerror(errno));   
    }  
	return ret;
}


int MsgQueue::getMsgId() {

	return sMsgQueueID;
}

