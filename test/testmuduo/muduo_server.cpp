#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;

/*muduo网络库开发服务器程序
1、组合TcpServer对象
2、创建EventLoop事件循环对象的指针
3、明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
4、在当前的构造函数中注册连接回调函数和读写事件处理函数
5、设置合适的线程数，muduo自己分配
*/
/*g++ -o server -g muduo_server.cpp -lmuduo_net -lmuduo_base -lpthread
bin lib include src build example thirdparty CMakeLists.txt autobuild.sh
*/
//编译产生的文件放到build里(cmake ..), 可执行文件放到bin(make)

class ChatServer
{
public:
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg)
        : _server(loop, listenAddr, nameArg)
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
    void start()
    {
        _server.start();
    }

private:
    // TcpServer绑定的回调函数，当有新连接或连接中断时调用
    void onConnection(const muduo::net::TcpConnectionPtr &con)
    {
        if (con->connected())
        {
            std::cout << con->peerAddress().toIpPort() << "->" << con->localAddress().toIpPort() << " state:on" << endl;
        }else
        {
            std::cout << con->peerAddress().toIpPort() << "->" << con->localAddress().toIpPort() << " state:off" << endl;
            con->shutdown();
            //_loop.quit();
        }
    }
    // TcpServer绑定的回调函数，当有新数据时调用
    void onMessage(const muduo::net::TcpConnectionPtr &con,
                   muduo::net::Buffer *buffer,
                   muduo::Timestamp time)
    {
        string buf = buffer ->retrieveAllAsString();
        cout << "recv data:" << buf << "time:" << time.toString() << endl;
        con->send(buf);
    }
    TcpServer _server; //1
    //EventLoop *_loop;   //2epoll
};

int main(){
    EventLoop loop;
    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop , addr, "ChatServer");

    server.start();
    loop.loop();//epoll_wait 以阻塞方式等待新用户的连接和已连接用户的读写事件

    return 0;
}