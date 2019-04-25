#include <libubox/uloop.h>
#include <libubox/ustream.h>
#include <libubox/utils.h>
#include <libubus.h>
#include <json/json.h>
#include <libubox/blobmsg_json.h>
#include <unistd.h>
#include <fcntl.h>

/*
//ubus上下文对象
struct ubus_context {
	struct list_head requests;
	struct avl_tree objects;
	struct list_head pending;

	struct uloop_fd sock;
	struct uloop_timeout pending_timer;

	uint32_t local_id;
	uint16_t request_seq;
	int stack_depth;

	void (*connection_lost)(struct ubus_context *ctx);

	struct ubus_msghdr_buf msgbuf;
	uint32_t msgbuf_data_len;
	int msgbuf_reduction_counter;
};
*/ 
static struct ubus_context * ctx = NULL;
static const char * cli_path;
/*
//ubus事件处理函数
struct ubus_event_handler {
	struct ubus_object obj;

	ubus_event_handler_t cb;
};
*/
static struct ubus_event_handler listener;
 
#define	UBUS_EVENT_ADD_DEVICE	"add_device"
#define	UBUS_EVENT_REMOVE_DEVICE	"rm_device"
 
static void ubus_probe_device_event(struct ubus_context *ctx, struct ubus_event_handler *ev,
			  const char *type, struct blob_attr *msg)
{
	char *str;
 
	//消息类型
	printf("type = %s\n",type);
	
	//用于blobmsg_json用于json对象的序列化
	str = blobmsg_format_json(msg, true);

	printf("str = %s\n",str);
	
	free(str);
	str = NULL;
}
 
static int client_ubus_register_events()
{	
	int ret = 0;
 
	/* 注册特定event的listener。多个event可以使用同一个listener */
	memset(&listener, 0, sizeof(listener));
	//绑定事件处理函数
	listener.cb = ubus_probe_device_event;
	
	/*
	int ubus_register_event_handler(struct ubus_context *ctx,
				struct ubus_event_handler *ev,
				const char *pattern);
	*/
	ret = ubus_register_event_handler(ctx, &listener, UBUS_EVENT_ADD_DEVICE);
	if (ret)
	{
		fprintf(stderr, "register event failed.\n");
		return -1;
	}
 
	ret = ubus_register_event_handler(ctx, &listener, UBUS_EVENT_REMOVE_DEVICE);
	if (ret)
	{
		ubus_unregister_event_handler(ctx, &listener);
		fprintf(stderr, "register event failed.\n");
		return -1;
	}
 
	return 0;
}
 
static void ubus_add_fd(void)
{
	/*
		原型:static inline void ubus_add_uloop(struct ubus_context *ctx)
		作用:把创建的ubus连接到注册的epoll中
	*/
	ubus_add_uloop(ctx);
 
#if 0
	printf("use FD_CLOEXEC\n");
	fcntl(ctx->sock.fd, F_SETFD,fcntl(ctx->sock.fd, F_GETFD) | FD_CLOEXEC);
#else
	printf("don'use FD_CLOEXEC\n");
#endif
}
 
static void ubus_reconn_timer(struct uloop_timeout *timeout)
{
	/*
		struct uloop_timeout
		{
			struct list_head list;
			bool pending;

			uloop_timeout_handler cb;
			struct timeval time;
		};
	*/
	static struct uloop_timeout retry =
	{
		.cb = ubus_reconn_timer,
	};
	int t = 2;
 
	if (ubus_reconnect(ctx, cli_path) != 0) {
		printf("reconnect start\n");
		uloop_timeout_set(&retry, t * 1000);
		return;
	}
 
	printf("reconnect end\n");
	ubus_add_fd();
	{
		ubus_unregister_event_handler(ctx, &listener);
		ubus_register_event_handler(ctx, &listener, UBUS_EVENT_ADD_DEVICE);
		ubus_register_event_handler(ctx, &listener, UBUS_EVENT_REMOVE_DEVICE);
	}
}
 
static void ubus_connection_lost(struct ubus_context *ctx)
{
	ubus_reconn_timer(NULL);
}
 
static int client_ubus_init(const char *path)
{
	/*
		原型:int uloop_init(void);
		作用:创建epoll句柄,最多监听32个fd
	*/
	uloop_init();
	
	cli_path = path;
	
	/*
		原型:struct ubus_context *ubus_connect(const char *path);
		作用:创建ubus连接
	*/
	ctx = ubus_connect(path);
	if(!path)
	{
		printf("path = %s\n",path);
	}
	
	if(!ctx)
	{
		printf("ubus connect failed\n");
		return -1;
	}
	
	printf("connected as %08x\n", ctx->local_id);
	
	//设置连接断开的回调函数
	ctx->connection_lost = ubus_connection_lost;
 
	ubus_add_fd();
 
	return 0;
}
 
static void client_ubus_done(void)
{
	if (ctx) {
		//关闭ubus连接
		ubus_free(ctx);
		uloop_done();
	}
		
}
 
int main(int argc, char * argv[])
{
	char * path = NULL;
 
	/* 连接ubusd */
	if (UBUS_STATUS_OK != client_ubus_init(path))
	{
		printf("ubus connect failed!\n");
		return -1;
	}
 
	/* 注册某个事件的处理函数 */
	client_ubus_register_events();
 
	//等待I/O事件发生,调用相对应的对象的功能函数
	uloop_run();
	
	client_ubus_done();
 
	return 0;
}
