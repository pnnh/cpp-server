//
// Created by larry on 17-11-2.
//

#include <msgpack.hpp>
#include "server.h"

Server::Server(unsigned short port_num) : _io_service(),
_endpoint(boost::asio::ip::tcp::v4(), port_num),
_acceptor(_io_service, _endpoint)
{
}

int Server::Serve()
{
    _acceptor.listen();
    Accept();
    _io_service.run();
    return 0;
}

Accepter* Server::Accept() {
    auto socket = new boost::asio::ip::tcp::socket(_io_service);
    _acceptor.async_accept(*socket, Accepter(this, socket));
};

msgpack::unpacker unp;

void do_read(boost::asio::ip::tcp::socket *socket, size_t length) {
    unp.reserve_buffer(length);
    auto reader = Reader(socket);
    socket->async_read_some(boost::asio::buffer(unp.buffer(), length), reader);
}


void do_read2(boost::asio::ip::tcp::socket *socket) {
}


void Accepter::operator()(boost::system::error_code ec) {
    std::cout<<"connected"<<std::endl;
    if(!ec) {
        Read();
        _server->Accept();
    } else std::cerr << "accept" << ec.message() << std::endl;
}



void HeadReader::operator()(boost::system::error_code ec, std::size_t size) {
    std::cout<<"head reading "<< size <<std::endl;
    if (!ec) {
        uint8_t type = buffer2[0];
        uint8_t flags = buffer2[1];
        std::cout<<"head yyy"<< ((uint32_t)buffer2[2] << 16) <<std::endl;
        uint32_t length = buffer2[2];
        length = (length << 8 | buffer2[3]) << 8 | buffer2[4];
        uint32_t stream = buffer2[5];
        stream = (stream << 8 | buffer2[6]) << buffer2[7];

        std::cout<<"head xxx"<< (int)type << " " << (int)flags << " " << length << " " << stream <<std::endl;

        do_read(_socket, length);
    } else std::cerr << "read error " << ec.message() << size << std::endl;
}

void Accepter::Read() {
    auto headReader = HeadReader(_socket);
    boost::asio::async_read(*_socket,
                            boost::asio::buffer(headReader.buffer2, HeadReader::length),
                            HeadReader::condition, headReader);
}



void Reader::operator()(boost::system::error_code ec, std::size_t size) {
    std::cout<<"reading "<< size <<std::endl;
    if (!ec) {
        unp.buffer_consumed(size);
        msgpack::object_handle oh;
        while (unp.next(oh)) {
            std::cout << "<---" << oh.get() << std::endl;
        }

        //do_read(_socket);


    } else std::cerr << "read error " << ec.message() << size << std::endl;
}