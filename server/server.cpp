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

boost::system::error_code check_error(std::string tag, boost::system::error_code ec) {
    if (ec) std::cerr << tag << ec.message() << std::endl;
    return ec;
}

Accepter* Server::Accept() {
    auto accepter = new Accepter(this, _io_service);
    _acceptor.async_accept(*accepter -> Socket(), [accepter](boost::system::error_code ec) {
        if (check_error("接受连接出错", ec)) return;
        accepter -> readHeader();
    });
};


Header parse_header(uint8_t *buffer) {
    uint8_t type = buffer[0];
    uint8_t flags = buffer[1];
    uint32_t length = buffer[2];
    length = (length << 8 | buffer[3]) << 8 | buffer[4];
    uint32_t stream = buffer[5];
    stream = (stream << 8 | buffer[6]) << buffer[7];

    return Header{type, flags, length, stream};
}

void parse_body(msgpack::unpacker *unp2, size_t size) {
    unp2->buffer_consumed(size);
    msgpack::object_handle oh;
    while (unp2->next(oh)) {
        std::cout << "<---" << oh.get() << std::endl;
    }
}


void Accepter::readHeader() {
    auto handler = [this](boost::system::error_code ec, std::size_t size) {
        if (check_error("读取消息头出错", ec)) return;
        auto header = parse_header(head_buffer());
        readBody(header.length);
    };
    boost::asio::async_read(*_socket,
                            boost::asio::buffer(_header_buffer, _header_length),
                            Accepter::header_condition,
                            handler);
}

Accepter::Accepter(Server *server, boost::asio::io_service& io_service) :
        _server(server) {
    _unp = new msgpack::unpacker();
    _socket = new boost::asio::ip::tcp::socket(io_service);
    _header_buffer = new uint8_t[_header_length];
};

void Accepter::readBody(size_t length) {
    _unp->reserve_buffer(length);
    auto condition = [length](const boost::system::error_code& error, std::size_t bytes_transferred) -> std::size_t {
        return bytes_transferred >= length ? 0 : length - bytes_transferred;
    };
    auto handler = [this](boost::system::error_code ec, std::size_t size) {
        if (check_error("读取消息体出错", ec)) return;
        parse_body(_unp, size);
        readHeader();
    };
    boost::asio::async_read(*_socket,
                            boost::asio::buffer(_unp -> buffer(), length),
                            condition, handler);
}

