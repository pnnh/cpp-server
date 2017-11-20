//
// Created by larry on 17-11-2.
//

#include <msgpack.hpp>
#include "server.h"

Server::Server(unsigned short port_num) :
_endpoint(boost::asio::ip::tcp::v4(), port_num),
_acceptor(_io_service, _endpoint),
_work(_io_service)
{
}

//Server::~Server(){
//    for (auto &_connection : _connections) {
//        delete _connection;
//    }
//}

void Server::Serve()
{
    //_acceptor.listen();
    Accept();

    auto threads_num = std::thread::hardware_concurrency() - 1;
    while(threads_num--) {
        _threads.emplace_back([this] () { _io_service.run(); });
    }
    _io_service.run();
}

void Server::remove(uint32_t id) {
    _connections.erase(id);
}

boost::system::error_code check_error(const std::string &tag, boost::system::error_code ec) {
    if (ec) std::cerr << tag << ec.message() << std::endl;
    return ec;
}

void Server::Accept() {
    auto connection = std::make_shared<Connection>(*this, _io_service, _connection_id);
    _connections[_connection_id++] = connection;
    _acceptor.async_accept(connection -> Socket(), [this, connection](boost::system::error_code ec) {
        if (check_error("接受连接出错", ec)) return;
        connection -> readHeader();
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

void parse_body(msgpack::unpacker &unp2, size_t size) {
    unp2.buffer_consumed(size);
    msgpack::object_handle oh;
    while (unp2.next(oh)) {
        std::cout << "<---" << oh.get() << std::endl;
    }
}

void Connection::readHeader() {
    auto handler = [this](boost::system::error_code ec, std::size_t size) {
        if (check("读取消息头出错", ec)) return;
        auto header = parse_header(head_buffer());
        readBody(header.length);
    };
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_header_buffer, _header_length),
                            Connection::header_condition,
                            _strand.wrap(handler));
}

std::size_t Connection::header_condition(const boost::system::error_code &error, std::size_t bytes_transferred) {
    if (error) return 0;
    return bytes_transferred >= _header_length ? 0 : _header_length - bytes_transferred;
}

Connection::Connection(Server &server, boost::asio::io_service& io_service, uint32_t id) :
        _server(server), _socket(io_service), _strand(io_service), _id(id) {
    //_socket = new boost::asio::ip::tcp::socket(io_service);
    //_header_buffer = uint8_t[_header_length];
    //_unp = new msgpack::unpacker();
};

//Connection::~Connection() {
//    //delete _socket;
//    delete _header_buffer;
//    delete _unp;
//}

void Connection::readBody(size_t length) {
    _unp.reserve_buffer(length);
    auto condition = [length](const boost::system::error_code& error, std::size_t bytes_transferred) -> std::size_t {
        if (error) return 0;
        return bytes_transferred >= length ? 0 : length - bytes_transferred;
    };
    auto handler = [this](boost::system::error_code ec, std::size_t size) {
        if (check("读取消息体出错", ec)) return;
        parse_body(_unp, size);
        readHeader();
    };
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_unp.buffer(), length),
                            condition, _strand.wrap(handler));
}

boost::system::error_code Connection::check(const std::string &tag, boost::system::error_code ec) {
    if (!check_error(tag, ec)) return {};
    _server.remove(_id);
    return ec;
}
