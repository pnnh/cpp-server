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
    boost::asio::io_service* Service() { return io_service; }

private:
    boost::asio::io_service *io_service;
    boost::asio::ip::tcp::endpoint *endpoint;
    boost::asio::ip::tcp::acceptor *acceptor;


    //void accept_handler(boost::system::error_code ec);
};

class Accepter {
public:
    Accepter(Server *serv ) :
            server(serv) {
        socket = new boost::asio::ip::tcp::socket(*serv->Service());
    };
    void Read();
    void operator()(boost::system::error_code ec);
    boost::asio::ip::tcp::socket* Socket() { return socket; }
private:
    Server *server;
    boost::asio::ip::tcp::socket *socket;
};


class Reader {
public:
    Reader(boost::asio::ip::tcp::socket *socket) : _socket(socket) {
        buffer2 = new char[4096];
    }
    void operator()(boost::system::error_code, std::size_t);
    //boost::array<char, 40960>* Buffer() { return &buff; }

    char* Buffer2() { return buffer2; };
private:
    //Accepter *accepter;
    boost::asio::ip::tcp::socket *_socket;
    boost::array<char, 40960> buff;

    char *buffer2;
    //boost::asio::mutable_buffer* buffer;
};





class HeadReader {
public:
    const static size_t length = 3;
    HeadReader(boost::asio::ip::tcp::socket *socket) : _socket(socket) {
        buffer2 = new uint8_t[length];
        buffer3 = new char[length];
    }
    void operator()(boost::system::error_code, std::size_t);

    static std::size_t condition(const boost::system::error_code& error, std::size_t bytes_transferred) {

        std::cout<<"bytes_transferred "<< error << "  " << bytes_transferred <<std::endl;
        return bytes_transferred >= length ? 0 : length - bytes_transferred;
    }

    uint8_t *buffer2;
    char *buffer3;
private:
    //Accepter *accepter;
    boost::asio::ip::tcp::socket *_socket;
};

#endif //CPPDEMO_SERVER_H
