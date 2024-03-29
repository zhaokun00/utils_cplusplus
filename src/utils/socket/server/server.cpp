#include "UnixSocket.h"
#include <pthread.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <netinet/in.h>

/*
在linux的网络编程中,很长的时间都在使用select来做事件触发,在linux新的内核中,有了一种替换他的机制,就是epoll,相比于
select、epoll最大的好处在于它不会随着监听fd数目的增长而降低效率,因为在内核中的select实现中,他是采用轮询来处理的
*/
#define PATH "/home/zhaokun/utils/utils_cplusplus/src/utils/socket/test"

#define MAX_EVENTS 500

//事件数组
struct epoll_event eventList[MAX_EVENTS];

class Message {

	public:
		int 	mId;
		char 	mName[10];
		float 	mScore;
		long 	mCount;
};

void * serverThreadProcess(void *obj) {

	UnixSocketServer server;

	server.openSocket(PATH,100);
	//epoll描述符
	int epollfd = 0;

	/*
		int epoll_create(int size);
		创建一个epoll的句柄,size用来告诉内核这个监听的数目一共有多大,这个参数不同于select()中的第一个参数,给出最大监听的fd+1的值。
		需要注意的是,当创建好epoll句柄后,它就会占用一个fd,在linux下如果查看/proc/进程号/fd/是能够看到这个fd,所以使用完epoll后,必须
		close()关闭,否则可能导致fd被耗尽
	*/
	epollfd = epoll_create(MAX_EVENTS);

	if(epollfd < 0) {
		printf("epoll_create failed\n");
	}

	/*
		int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
		epoll的事件注册函数,它不同于select实在监听事件时告诉内核要监听什么类型的事件,而是在这里先注册要监听的事件类型
		epfd:epoll_create()的返回值
		op:动作,用3个宏来表示:
			EPOLL_CTL_ADD：注册新的fd到epfd中；
			EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
			EPOLL_CTL_DEL：从epfd中删除一个fd；
		fd:需要监听的fd
		event:告诉内核需要监听什么事

		struct epoll_event {
		  __uint32_t events;  
		  epoll_data_t data;  
		};

		events可以是以下几个宏的集合：
		EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
		EPOLLOUT：表示对应的文件描述符可以写；
		EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
		EPOLLERR：表示对应的文件描述符发生错误；
		EPOLLHUP：表示对应的文件描述符被挂断；
		EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
		EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
	*/

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = server.m_socketFd;

    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, server.m_socketFd, &event) < 0) {
        printf("epoll add fail : fd = %d\n", server.m_socketFd);
	}

 	int timeout = 3000;  
	while(1) {
        
		/*
			int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
			等待事件的产生,类似于select()调用,
			参数events用来从内核得到事件的集合
			maxevents告知这个events有多大，这个maxevents的值不能大于创建epoll_create()时的size
			timeout是超时时间(单位毫秒,0会立即返回,-1将永久阻塞)
			返回值:需要处理的事件数目,如果返回0表示超时,小于0表示发生错误
		*/              
        int ret = epoll_wait(epollfd, eventList, MAX_EVENTS, timeout);
        
        if(ret < 0) { //发生错误,退出循环
            printf("epoll error\n");
            break;
        }
        else if(ret == 0) { //表示超时
            printf("timeout ...\n");
            continue;
        }

		//表示有事件发生
        for( int i=0; i<ret; i++)
        {
            if ((eventList[i].events & EPOLLERR) || (eventList[i].events & EPOLLHUP) || !(eventList[i].events & EPOLLIN)) { //错误退出
              printf ( "epoll error\n");
              close (eventList[i].data.fd);
            }
            
            if (eventList[i].data.fd == server.m_socketFd) {

            	printf("serverl wait connect...\n");

				int clientFd = server.acceptSocket();
        
				struct epoll_event ev;
				ev.data.fd = clientFd;
				ev.events =  EPOLLIN | EPOLLET;

				//将文件描述符注册到内核中
				epoll_ctl(epollfd, EPOLL_CTL_ADD, clientFd, &ev);
            }
			else {
				printf("server端接收到了数据\n");
				int clientFd = eventList[i].data.fd;
				printf("clientFd = %d\n",clientFd);
				
				Message msg;

				server.readSocket(clientFd,&msg,sizeof(msg)); //服务端读取数据

				msg.mId = clientFd;

				server.writeSocket(clientFd,&msg, sizeof(msg)); //服务端将读取的数据返回
            }
        }
    }

	server.closeSocket();
	close(epollfd);

	return NULL;
}

int main() {

	int ret = 0;
	pthread_t serverThead;
		
	pthread_create(&serverThead, NULL, serverThreadProcess, NULL);


	while(1) {
		sleep(1);
	}
	return ret;
}
