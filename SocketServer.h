//
// Created by zll on 16-8-11.
//

#ifndef CPPDEMO_SOCKETSERVER_H
#define CPPDEMO_SOCKETSERVER_H

#include <iostream>
#include <string>
#include <boost/asio.hpp>

using namespace boost::asio;

using boost::system::error_code;
using boost::asio::io_service;
using boost::asio::ip::tcp;

class SocketServer
{
public:
    //类的初始化创建：设置io_service, 设置端口
    SocketServer();

    void listen();

private:
     io_service service_;
     tcp::acceptor acceptor_;

    //创建一个tcp的socket；且还是侦听
    void process(std::shared_ptr<tcp::socket> socket);
};



#endif //CPPDEMO_SOCKETSERVER_H
