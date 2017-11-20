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
#include <thread>

class Connection;

class Server {
public:
    explicit Server(unsigned short port_num);
    void Serve();
    void Accept();
    void remove(uint16_t id);
    uint16_t generate_id();
private:
    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::endpoint _endpoint;
    boost::asio::ip::tcp::acceptor _acceptor;
    //uint32_t  _connection_id = 1;
    const static size_t _max_connections = 2;
    std::map<uint16_t, std::shared_ptr<Connection>> _connections;
    boost::asio::io_service::work _work;
    std::vector<std::thread> _threads;
};

class Connection {
public:
    Connection(Server &server, boost::asio::io_service& io_service, uint16_t id);
    void readHeader();
    void readBody(size_t length);
    msgpack::unpacker& unpacker() { return _unp; }
    boost::asio::ip::tcp::socket& Socket() { return _socket; }
    static std::size_t header_condition(const boost::system::error_code& error, std::size_t bytes_transferred);
    uint8_t * head_buffer() { return _header_buffer; }
    boost::system::error_code check(const std::string &tag, boost::system::error_code ec);
    uint16_t id() { return _id; }
private:
    Server &_server;
    boost::asio::ip::tcp::socket _socket;
    msgpack::unpacker _unp;
    const static size_t _header_length = 8;
    uint8_t _header_buffer[_header_length];
    uint16_t _id;
};

struct Header {
    uint8_t type;
    uint8_t flags;
    uint32_t length;
    uint32_t stream;
};

#endif //CPPDEMO_SERVER_H
