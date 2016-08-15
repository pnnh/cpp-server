//
// Created by zll on 16-8-11.
//

#include "SocketServer.h"
#include "SocketClient.h"


SocketServer::SocketServer()
:service_(), acceptor_(service_, tcp::endpoint(tcp::v4(), 7000))
{
}

void SocketServer::listen() {
    start();
    service_.run();
}

void SocketServer::start()
{
    // 开始等待连接（非阻塞）
     shared_ptr<tcp::socket> psocket(new tcp::socket(service_));

    // 触发的事件只有error_code参数，所以用boost::bind把socket绑定进去
    acceptor_.async_accept(*psocket, bind(&SocketServer::accept_handler, this, psocket, _1) );

}


// 有客户端连接时accept_handler触发
void SocketServer::accept_handler( shared_ptr<tcp::socket> psocket, error_code ec)
{
    if(ec) return;

    // 继续等待连接
    start();

    // 显示远程IP
    std::cout << psocket->remote_endpoint().address() << std::endl;

    SocketClient client(psocket);
    client.Write("hello hahahaha");
}