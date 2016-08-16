//
// Created by zll on 16-8-11.
//

#include "SocketServer.h"


SocketServer::SocketServer()
:service_(), acceptor_(service_,  tcp::endpoint(tcp::v4(), 7000))
{
}

void SocketServer::listen() {
    service_.run();

    for(;;){
        std::shared_ptr< tcp::socket> psocket(new tcp::socket(service_));

        acceptor_.accept(*psocket );

        process(psocket);
    }
}

void SocketServer::process(std::shared_ptr<tcp::socket> socket)
{
    using namespace boost::asio::error;
    for (;;){
//        std::array<unsigned char, 16> read_buffer;
//        size_t n = socket->read_some(buffer(read_buffer));
//        string str(read_buffer.begin(), read_buffer.begin() + n);
//        std::cout<<"read :"<<str <<std::endl;

        try {
            streambuf buffer(8);
            size_t n = read_until(*socket, buffer, '\0');

            streambuf::const_buffers_type bufs = buffer.data();
            std::string line( buffers_begin(bufs),  buffers_begin(bufs) + n-1);
            std::cout<< line <<std::endl;
        } catch (boost::system::system_error  e){
            std::cout << e.what()<<(e.code() == make_error_code( misc_errors::not_found)) <<std::endl;
        }
    }
}