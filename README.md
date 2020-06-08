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



