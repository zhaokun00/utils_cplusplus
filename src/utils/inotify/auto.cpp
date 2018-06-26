/*
inotify是一个Linux内核特性,它监控文件系统,并且及时向专门的应用程序发出相关的事件警告,比如删除、读、写和卸载操作等。
还可以跟踪活动的源头和目标等细节。在实际项目中,如果项目带有配置文件,那么怎么让配置文件的改变和项目程序同步而不需要
重启程序呢？一个明显的应用是:在一个程序中,使用inotify监视他的配置文件,如果该配置文件发生了改变(更新、修改)时,inotify会产生修改的事件
给程序,应用程序就可以实现重新加载配置文件,检测那些参数发生了变化,并在应用程序内存的一些变量做相应的修改

总结:它是一个内核用于通知用户空间程序文件系统变化的机制
*/ 
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <sys/inotify.h>  
#include <unistd.h>  
  
#define EVENT_NUM 12  

//inotify可以监视文件系统事件包括:
char *event_str[EVENT_NUM] =   
{  
    "IN_ACCESS", 			//文件被访问
    "IN_MODIFY",  			//文件被修改
    "IN_ATTRIB",  			//文件属性被修改,如chmod、chown、touch
    "IN_CLOSE_WRITE",       //可写文件被close
    "IN_CLOSE_NOWRITE",  	//不可写文件被close
    "IN_OPEN",  			//文件被open
    "IN_MOVED_FROM", 		//文件被移走
    "IN_MOVED_TO",  		//文件被移来
    "IN_CREATE",  			//文件被创建
    "IN_DELETE",  			//文件被删除
    "IN_DELETE_SELF",  		//自删除、即一个可执行文件在执行时删除自己
    "IN_MOVE_SELF"  		//自移动,即一个可执行文件在执行时移动自己
};  
  
//IN_UNMOUNT:宿主文件系统被 umount
//IN_CLOSE:文件被关闭，等同于(IN_CLOSE_WRITE | IN_CLOSE_NOWRITE)
//IN_MOVE:IN_MOVE，文件被移动，等同于(IN_MOVED_FROM | IN_MOVED_TO)

int test1() {

	int fd;  
    int wd;  
    int len;  
    int nread;  
    char buf[BUFSIZ];  
    struct inotify_event *event;  
    int i;  
      
	//在内核中创建inotify子系统的一个实例,成功的话将返回一个文件描述符,失败则返回-1,如果失败请检查errno以获得诊断信息
    fd = inotify_init();  
    if( fd < 0 )  
    {  
        fprintf(stderr, "inotify_init failed\n");  
        return -1;  
    }  
    
	/*用于添加监视器,每个监视器必须提供一个路径名和相关事件列表(每个事件由一个常量指定,比如IN_MODIFY)
	  要监控多个事件,只需要在事件之间使用逻辑操作符|,如果inotify_add_watch()成功,该调用会为已注册的监视器
	  返回一个唯一的标识符,否则返回-1,使用这个标识符更改或删除相关的监视器

	//添加事件
	int inotify_add_watch (int fd, const char *path, __u32 mask);

	//移除事件
	int inotify_rm_watch (int fd, __u32 mask);

	*/
    wd = inotify_add_watch(fd, "../test", IN_ALL_EVENTS);  
    if(wd < 0)  
    {  
        fprintf(stderr, "inotify_add_watch %s failed\n", "test");  
        return -1;  
    }  
      
    buf[sizeof(buf) - 1] = 0;
	/*
	文件事件用一个inotify_event结构表示,它通过由inotify_init()返回的文件描述符使用文件读取函数read来获得
	struct inotify_event {
		    __s32           wd;   //被监视目标的watch描述符
		    __u32           mask; //mask为事件掩码,len为name字符串的长度,name为被监视
		    __u32           cookie;
		    __u32           len; //len为name字符串的长度
		    char            name[0]; //name为被监视目标的路径名,它只是为用户方面
	};

	size_t len = read (fd, buf, BUF_LEN);
	buf是一个inotify_event结构的数组指针,BUF_LEN指定要读取的总长度,buf大小至少要不小于BUF_LEN，该调用返回的事件数取决于BUF_LEN以及时间中
	文件名的长度.len为实际读取的字节数,即获得的事件的总长度

	可以在函数inotify_init()返回的文件描述符fd上使用select或者poll,也可以在fd上使用ioctl命令FIONREAD来得到当前队列的长度。close(fd)将删除
	所有添加到fd的watch并做必要的清理

	
	*/
    while((len = read(fd, buf, sizeof(buf) - 1)) > 0) //默认为read为阻塞函数,读取事件
    {  
		printf("len = %d\n",len);        
		nread = 0;  
        while( len > 0 )  
        {  
            printf("进来了一次\n");
			event = (struct inotify_event *)&buf[nread]; //

            for(i=0; i<EVENT_NUM; i++)  
            {  
                if((event->mask >> i) & 1)  
                {  
                    if(event->len > 0) {
						printf("event->len > 0");
						fprintf(stdout, "%s --- %s\n", event->name, event_str[i]);
					} 
                    else {
						printf("event->len < 0");
						fprintf(stdout, "%s --- %s\n", " ", event_str[i]);  
					}       
                }  
            }  
            nread = nread + sizeof(struct inotify_event) + event->len;  
            len = len - sizeof(struct inotify_event) - event->len;  
        }  
    }
}

void test2() {

#define EVENT_SIZE  ( sizeof (struct inotify_event) )  
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

	int length, i = 0;  
    int fd;  
    int wd;  
    char buffer[EVENT_BUF_LEN];  
  
    fd = inotify_init();  
  
    if (fd < 0) {  
        perror("inotify_init");  
    }  
  
    wd = inotify_add_watch(fd, "../test", IN_CREATE | IN_DELETE | IN_MODIFY);  //监听创建、删除、修改
//    inotify_add_watch(fd, "/tmp/dir1", IN_CREATE | IN_DELETE);  //添加了一个子目录进行监听，默认是不会监听子目录的   
  
    for ( ; ;)
    {  
        i = 0;  
        length = read(fd, buffer, EVENT_BUF_LEN);  
          
        while (i < length) {  
            struct inotify_event *event = (struct inotify_event *) &buffer[i];  
            if (event->len) {  
                if(event->mask & IN_CREATE) {  
                    if (event->mask & IN_ISDIR) {  
                        printf("New directory %s created.\n", event->name);  
                    } else {  
                        printf("New file %s created.\n", event->name);  
                    }  
                }else if (event->mask & IN_DELETE) {  
                    if (event->mask & IN_ISDIR) {  
                        printf("Directory %s deleted.\n", event->name);  
                    } else {  
                        printf("File %s deleted.\n", event->name);  
                    }  
                }else if (event->mask & IN_MODIFY) {  
              		printf("File %s modify.\n", event->name);   
                } 
            }  
            i += EVENT_SIZE + event->len;  
        }
    }  
 
    inotify_rm_watch(fd, wd);  
   
    close(fd);
}

int main(int argc, char *argv[])  
{  
     
//    test1();

	test2();
    return 0;  
}  

