//
// Created by zll on 16-8-11.
//

#ifndef CPPDEMO_SOCKETSERVER_H
#define CPPDEMO_SOCKETSERVER_H

#include <iostream>
#include <string>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace std;

using boost::system::error_code;
using ip::tcp;

class SocketServer
{
public:
    //类的初始化创建：设置io_service, 设置端口
    SocketServer();

    void listen();

    // 有客户端连接时accept_handler触发
    void accept_handler(shared_ptr<tcp::socket> psocket, error_code ec);

private:
    io_service service_;
    ip::tcp::acceptor acceptor_;

    //创建一个tcp的socket；且还是侦听
    void start();
};



#endif //CPPDEMO_SOCKETSERVER_H
