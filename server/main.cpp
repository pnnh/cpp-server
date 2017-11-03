//#include <string>
//#include <boost/asio.hpp>
//#include <iostream>
//#include <boost/array.hpp>
//#include <msgpack.hpp>
#include "server.h"
//
//boost::asio::io_service io_service;
//boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 7000);
//boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint);
//boost::asio::ip::tcp::socket sock(io_service);
//boost::array<char, 40960> buffer;
//
//void read_handler(boost::system::error_code ec, std::size_t size);
//
//void read() {
//    sock.async_read_some(boost::asio::buffer(buffer), read_handler);
//}
//
//void write_handler(boost::system::error_code ec, std::size_t size) {
//    if (ec) {
//        std::cerr << "write" << ec.message() << size << std::endl;
//    }
//    read();
//}
//
//void read_handler(boost::system::error_code ec, std::size_t size) {
//    if (!ec) {
//
//
//
//        msgpack::unpacker pac;
//        // feeds the buffer.
//        pac.reserve_buffer(size);
//        memcpy(pac.buffer(), buffer.data(), size);
//        pac.buffer_consumed(size);
//
//        // now starts streaming deserialization.
//        msgpack::object_handle oh;
//        while(pac.next(oh)) {
//            std::cout << "<--" << oh.get() << std::endl;
//        }
//
//        //std::cout << "<==" << std::string(buffer.data()) << std::endl;
//
//
//        std::string data = std::string(buffer.data(), size);
//        boost::asio::async_write(sock, boost::asio::buffer(data), write_handler);
//    } else std::cerr << "read" << ec.message() << size << std::endl;
//}
//
//void accept_handler(boost::system::error_code ec) {
//    if(!ec) {
//        //sock.async_read_some(boost::asio::buffer(buffer), read_handler);
//        read();
//    } else std::cerr << "accept" << ec.message() << std::endl;
//}

int main() {
    Server server(7000);
    server.Serve();
}