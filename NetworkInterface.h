#ifndef __NETWORKINTERFACE_H_
#define __NETWORKINTERFACE_H_

#include <event.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <string>

#include "glo_def.h"
#include "ievent.h"

#define MESSAGE_HEADER_LEN 10
#define MESSAGE_HEADER_ID "FBEB"

enum class SESSION_STATUS
{
	SS_REQUEST,
	SS_RESPONSE
};

enum class MESSAGE_STATUS
{
	MS_READ_HEADER = 0,
	MS_READ_MESSAGE = 1, //消息传输未开始
	MS_READ_DONE = 2,	 //消息传输完毕
	MS_SENDING = 3		 //消息传输中
};

typedef struct _ConnectSession {//连接会话 结构体，保存每个连接的状态信息
	char remote_ip[32];

	SESSION_STATUS session_stat;

	iEvent* request;
	MESSAGE_STATUS req_stat;

	iEvent* response;
	MESSAGE_STATUS res_stat;

	i32 eid;	//保存当前请求的事件id
	i32 fd;		//保存当前传送的文件句柄

	struct bufferevent* bev;//缓存去
	u32 message_len;        //已经读写消息的长度
	u32 read_message_len;   //已经读取的消息长度
	
	char* read_buf; //保存读消息的缓冲区
	char header[MESSAGE_HEADER_LEN + 1];//保存头部，10字节+1字节
	char *write_buf;
	u32 sent_len;   //已经发送的长度
	u32 read_header_len; //已读取的头部长度
}ConnectSession;

class NetworkInterface
{
public:
	NetworkInterface();
	~NetworkInterface();

	bool start(int port);
	void close();

	void session_reset(ConnectSession* cs);//重置连接会话状态
	static void listener_cb(struct evconnlistener* listener, evutil_socket_t fd,
		struct sockaddr* sock, int socklen, void* arg);//监听回调函数，接收连接请求

	static void handle_request(struct bufferevent* bev, void* arg); //读请求回调
	static void handle_response(struct bufferevent* bev, void* arg);//回复请求回调
	static void handle_error(struct bufferevent* bev, short event, void* arg);

	void network_event_dispatch();//网络事件分发函数，处理响应事件，回复响应消息
	void send_response_message(ConnectSession* cs);//把响应消息发送给客户端

private:
	struct evconnlistener* listener_;//libevent监听器对象
	struct event_base* base_;//libevent事件循环对象

	ConnectSession* m_cs;
};

#endif // !BIK_INTERFACE_NETWORK_INTERFACE_H_