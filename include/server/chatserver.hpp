//@Author Xuhang Zhang
//@Email xxx
#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;
//聊天服务器主类
class ChatServer
{
public:
    //初始化聊天服务器对象
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg);
    //启动服务
    void start();

private:
    //连接回调函数
    void onConnection(const muduo::net::TcpConnectionPtr &);
    //消息处理回调函数
    void onMessage(const muduo::net::TcpConnectionPtr &,
                   muduo::net::Buffer *,
                   muduo::Timestamp);

private:
    TcpServer _server;
    EventLoop *_loop;
};

#endif