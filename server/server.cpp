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
Server::~Server(){
    for (auto &_connection : _connections) {
        delete _connection;
    }
}

void Server::Serve()
{
    _acceptor.listen();
    Accept();
    _io_service.run();
}

void Server::remove(Connection *connection) {
    _connections.remove(connection);
    delete connection;
}

boost::system::error_code check_error(const std::string &tag, boost::system::error_code ec) {
    if (ec) std::cerr << tag << ec.message() << std::endl;
    return ec;
}

void Server::Accept() {
    auto connection = new Connection(this, _io_service);
    _connections.push_back(connection);
    _acceptor.async_accept(*connection -> Socket(), [this](boost::system::error_code ec) {
        if (check_error("接受连接出错", ec)) return;
        _connections.back() -> readHeader();
        Accept();
    });
};


Header parse_header(const uint8_t *buffer) {
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

void Connection::readHeader() {
    auto handler = [this](boost::system::error_code ec, std::size_t size) {
        if (check("读取消息头出错", ec)) return;
        auto header = parse_header(head_buffer());
        readBody(header.length);
    };
    boost::asio::async_read(*_socket,
                            boost::asio::buffer(_header_buffer, _header_length),
                            Connection::header_condition,
                            handler);
}

std::size_t Connection::header_condition(const boost::system::error_code &error, std::size_t bytes_transferred) {
    if (error) return 0;
    return bytes_transferred >= _header_length ? 0 : _header_length - bytes_transferred;
}

Connection::Connection(Server *server, boost::asio::io_service& io_service) :
        _server(server) {
    _socket = new boost::asio::ip::tcp::socket(io_service);
    _header_buffer = new uint8_t[_header_length];
    _unp = new msgpack::unpacker();
};

Connection::~Connection() {
    std::cerr << "connec 析构函数被调用" << std::endl;
    delete _socket;
    delete _header_buffer;
    delete _unp;
}

void Connection::readBody(size_t length) {
    _unp->reserve_buffer(length);
    auto condition = [length](const boost::system::error_code& error, std::size_t bytes_transferred) -> std::size_t {
        if (error) return 0;
        return bytes_transferred >= length ? 0 : length - bytes_transferred;
    };
    auto handler = [this](boost::system::error_code ec, std::size_t size) {
        if (check("读取消息体出错", ec)) return;
        parse_body(_unp, size);
        readHeader();
    };
    boost::asio::async_read(*_socket,
                            boost::asio::buffer(_unp -> buffer(), length),
                            condition, handler);
}

boost::system::error_code Connection::check(const std::string &tag, boost::system::error_code ec) {
    if (!check_error(tag, ec)) return {};
    _server -> remove(this);
    return ec;
}
