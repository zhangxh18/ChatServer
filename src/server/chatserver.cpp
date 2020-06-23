#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"

#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
//,_loop(loop)
{
    //给服务器注册用户连接的创建和断开回调
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    //给服务器注册用户读写事件的回调
    _server.setMessageCallback(bind(&ChatServer::onMessage,
                                    this, _1, _2, _3));

    // 设置EventLoop的线程个数 1个I/O线程 3个Worker线程
    _server.setThreadNum(4);
}

// 启动ChatServer服务
void ChatServer::start()
{
    _server.start();
}

// TcpServer绑定的回调函数，当有新连接或连接中断时调用
void ChatServer::onConnection(const muduo::net::TcpConnectionPtr &conn)
{
    if(!conn->connected()){
        ChatService::instance() -> clientCloseException(conn);
        conn->shutdown();
    }
}
// TcpServer绑定的回调函数，当有新数据时调用
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buffer,
                           Timestamp time)
{
    string buf = buffer->retrieveAllAsString();

    // 测试，添加json打印代码
    cout << buf << endl;

    // 数据的反序列化
    json js = json::parse(buf);
    // 达到的目的：完全解耦网络模块的代码和业务模块的代码
    // 通过js["msgid"] 获取=》业务handler=》conn  js  time
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
}