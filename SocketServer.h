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

class CHelloWorld_Service
{
public:
    //类的初始化创建：设置io_service, 设置端口
    CHelloWorld_Service(io_service &iosev);

    //创建一个tcp的socket；且还是侦听
    void start();

    // 有客户端连接时accept_handler触发
    void accept_handler( shared_ptr<tcp::socket> psocket, error_code ec);



private:
    io_service &m_iosev;
    ip::tcp::acceptor m_acceptor;
};



#endif //CPPDEMO_SOCKETSERVER_H
