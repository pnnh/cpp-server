//
// Created by larry on 17-11-2.
//

#include <msgpack.hpp>
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
    Accept();
     io_service->run();
    return 0;
}

Accepter* Server::Accept() {
    //Accepter accepter(this);
    auto accepter = Accepter(this);
    //acceptor->async_accept(*accepter.Socket(), accepter);
    acceptor->async_accept(*accepter.Socket(), accepter);
};

void Accepter::operator()(boost::system::error_code ec) {
    std::cout<<"connected"<<std::endl;
    if(!ec) {
        Read();
        //server->Accept();
    } else std::cerr << "accept" << ec.message() << std::endl;
}

msgpack::unpacker unp;
//std::size_t const window_size = 50;

void do_read(boost::asio::ip::tcp::socket *socket, size_t length) {
    unp.reserve_buffer(length);
    auto reader = Reader(socket);
    socket->async_read_some(boost::asio::buffer(unp.buffer(), length), reader);
}


void do_read2(boost::asio::ip::tcp::socket *socket) {
    auto headReader = HeadReader(socket);
    boost::asio::async_read(*socket,
                            boost::asio::buffer(headReader.buffer2, HeadReader::length),
        HeadReader::condition, headReader);
}


void HeadReader::operator()(boost::system::error_code ec, std::size_t size) {
    std::cout<<"head reading "<< size <<std::endl;
    if (!ec) {

        std::cout<<"head xxx"<< (int)buffer2[0] << (int)buffer2[1]<<(int)buffer2[2]
                <<(int)buffer2[3]<<(int)buffer2[4]<<(int)buffer2[5]
                <<(int)buffer2[6]<<(int)buffer2[7]<<std::endl;

        auto type = (uint8_t)buffer2[0];
        auto flags = (uint8_t)buffer2[1];
        std::cout<<"head yyy"<< ((uint32_t)buffer2[2] << 16) <<std::endl;
        uint32_t length = (uint32_t)buffer2[2] << 16 | (uint32_t)buffer2[3] << 8 |
                (uint32_t)buffer2[4];
        uint32_t stream = (uint32_t)buffer2[5] << 16 | (uint32_t)buffer2[6] << 8 |
                          (uint32_t)buffer2[7];

        std::cout<<"head xxx"<< (int)type << " " << (int)flags << " " << length << " " << stream <<std::endl;
        //do_read2(_socket);

        do_read(_socket, length);

        //std::cout<<"head yyyy"<< buffer3[0] << buffer3[1]<<buffer3[2] <<std::endl;
    } else std::cerr << "read error " << ec.message() << size << std::endl;
}

void do_read3(boost::asio::ip::tcp::socket *socket) {
    //boost::array<size_t , 2> buff;
    //boost::asio::async_read(socket, )
}

void Accepter::Read() {
    //auto reader = Reader(this -> socket);
    //socket->async_read_some(boost::asio::buffer(reader.Buffer2(), 4096), reader);

    do_read2(socket);
    //do_read(socket);
}



void Reader::operator()(boost::system::error_code ec, std::size_t size) {
    std::cout<<"reading "<< size <<std::endl;
    if (!ec) {
        //std::string data = std::string(buffer2, size);
        //std::cout << "<==" << data << std::endl;

        //auto reader = Reader(_socket);
        //_socket->async_read_some(boost::asio::buffer(reader.Buffer2(), 4096), reader);

        unp.buffer_consumed(size);
        msgpack::object_handle oh;
        while (unp.next(oh)) {
            std::cout << "<---" << oh.get() << std::endl;
            // In order to finish the program,
            // return if one complete msgpack is processed.
            // In actual server, don't return here.
            //return;
        }

        //do_read(_socket);


    } else std::cerr << "read error " << ec.message() << size << std::endl;
}