# ChatServer
本项目为工作在nginx tcp负载均衡环境中的集群聊天服务器和客户端--基于muduo实现
* 基于muduo的客户端服务器编程
* 服务器集群：nginx配置tcp负载均衡
* 服务器中间件：基于发布-订阅的Redis
* MySQL数据库编程
* Json序列化和反序列化

## 编译方式
```
cd build
rm -rf *
cmake ..
make
```
## TCP负载均衡简单配置
```
stream {
    upstream MyServer {
        server 127.0.0.1:6000 weight=1 max_fails=3 fail_timeout=30s;
        server 127.0.0.1:6002 weight=1 max_fails=3 fail_timeout=30s;
    }
    server {
        proxy_connect_timeout 1s;
        listen 8888;
        proxy_pass MyServer;
        tcp_nodelay on;
    }
}

```

## 使用示例
服务器按配置启动；
客户端使用如下：
```
zxh@xxx:xxx/bin$ ./ChatClient 127.0.0.1 8888
========================
1. login
2. register
3. quit
========================
choice:1    
userid:123456
userpassword:666666
"用户名或密码错误"
========================
1. login
2. register
3. quit
========================
choice:1
userid:1    
userpassword:123456
======================login user======================
current login user => id:1 name:zhang san
----------------------friend list---------------------
4 li si offline
----------------------group list----------------------
1 C++ CHAT start a chat project
1 zhang san online creator
4 li si offline normal
======================================================
show command list >>> 
addgroup : 加入群组，格式addgroup:groupid
creategroup : 创建群组，格式creategroup:groupname:groupdesc
loginout : 注销，格式loginout
addfriend : 添加好友，格式addfriend:friendid
groupchat : 群聊，格式groupchat:groupid:message
chat : 一对一聊天，格式chat:friendid:message
help : 显示所有支持的命令，格式help
```



