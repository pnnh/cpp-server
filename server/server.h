//
// Created by larry on 17-11-2.
//

#ifndef CPPDEMO_SERVER_H
#define CPPDEMO_SERVER_H

#include <string>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio/read.hpp>


#include <msgpack.hpp>

class Accepter;

class Server {
public:
    Server(unsigned short port_num);
    int Serve();
    Accepter* Accept();
private:
    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::endpoint _endpoint;
    boost::asio::ip::tcp::acceptor _acceptor;
};

class Accepter {
public:
    Accepter(Server *server, boost::asio::io_service& io_service);
    void readHeader();
    void readBody(size_t length);
    msgpack::unpacker* unpacker() { return _unp; }
    boost::asio::ip::tcp::socket* Socket() { return _socket; }

    static std::size_t header_condition(const boost::system::error_code& error, std::size_t bytes_transferred) {
        return bytes_transferred >= _header_length ? 0 : _header_length - bytes_transferred;
    }
    uint8_t * head_buffer() { return _header_buffer; }
private:
    Server *_server;
    boost::asio::ip::tcp::socket *_socket;
    msgpack::unpacker *_unp;
    const static size_t _header_length = 8;
    uint8_t *_header_buffer;
};

struct Header {
    uint8_t type;
    uint8_t flags;
    uint32_t length;
    uint32_t stream;
};

#endif //CPPDEMO_SERVER_H
