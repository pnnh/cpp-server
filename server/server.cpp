//
// Created by larry on 17-11-2.
//

#include "server.h"

Server::Server(unsigned short port_num)  //: io_service()//,
    //endpoint(boost::asio::ip::tcp::v4(), port_num)//,
    //acceptor(io_service, endpoint)
{
    io_service = new boost::asio::io_service();
    endpoint = new boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_num);
    acceptor = new boost::asio::ip::tcp::acceptor(*io_service, *endpoint);
}

//boost::asio::ip::tcp::socket& Server::Socket() {
//    return new boost::asio::ip::tcp::socket(io_service);
//}


void  accept_handler(boost::system::error_code ec) {
    std::cout<<"connected22222"<<std::endl;
    if(!ec) {
    } else std::cerr << "accept" << ec.message() << std::endl;
}


int Server::Serve()
{
    acceptor -> listen();
        //auto accepter = new Accepter(this);
    Accepter accepter(this);
//    auto accepter = this -> Accept();
//    //boost::asio::ip::tcp::socket sock(io_service);
 acceptor->async_accept(*accepter.Socket(), accepter);
//    //acceptor.async_accept(accepter -> Socket(), *accepter);

//    boost::asio::ip::tcp::socket sock(*io_service);
//    acceptor -> async_accept(sock, accept_handler);
     io_service->run();
    return 0;
}

Accepter* Server::Accept() {
    //return new Accepter(this, &sock);
};

//boost::array<char, 40960> buffer;
//char buffer[4096];
void Accepter::operator()(boost::system::error_code ec) {
    std::cout<<"connected"<<std::endl;
    if(!ec) {
        //auto reader = new Reader();
        Reader reader;
        //socket -> async_read_some(reader.Buffer(), read_handler);
        //socket->async_read_some(boost::asio::buffer(*reader->Buffer()), *reader);
        socket->async_read_some(boost::asio::buffer(reader.Buffer2(), 4096), reader);

        //socket->async_read_some(boost::asio::buffer(buffer), *reader);
        //socket->async_read_some(boost::asio::buffer(buffer, 4096), *reader);
    } else std::cerr << "accept" << ec.message() << std::endl;
}

void Reader::operator()(boost::system::error_code ec, std::size_t size) {
    std::cout<<"reading "<< size <<std::endl;
    if (!ec) {
        //std::string data = std::string(buff.data(), size);
        //std::string data = std::string(buffer.data(), size);
        //std::string data = std::string(buffer , size);
        std::string data = std::string(buffer2, size);
        std::cout << "<==" << data << std::endl;
    } else std::cerr << "read" << ec.message() << size << std::endl;
}