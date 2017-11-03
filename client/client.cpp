#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <msgpack.hpp>

boost::asio::io_service io_service;
boost::asio::ip::tcp::resolver resolver(io_service);
boost::asio::ip::tcp::socket sock(io_service);
boost::array<char, 40960> buffer;

void read_handler(boost::system::error_code ec, std::size_t size);

int n = 0;

void write_read() {
//    std::string str = "abc";
//
//    msgpack::sbuffer sbuf;
//    msgpack::packer<msgpack::sbuffer> pk(&sbuf);
//    pk.pack(std::string("Log message ... " + std::to_string(n++)));
//    pk.pack(std::string("=================="));
//
//    auto buf = boost::asio::buffer(sbuf.data(), sbuf.size());
//    boost::asio::write(sock, buf);

    std::string str = "abc";
    boost::asio::write(sock, boost::asio::buffer(str));

    sock.async_read_some(boost::asio::buffer(buffer), read_handler);
}

void read_handler(boost::system::error_code ec, std::size_t size) {
    if (!ec) {
        //std::cout << std::string(buffer.data(), size) << std::endl;
        //sock.async_read_some(boost::asio::buffer(buffer), read_handler);


        msgpack::unpacker pac;
        // feeds the buffer.
        pac.reserve_buffer(size);
        memcpy(pac.buffer(), buffer.data(), size);
        pac.buffer_consumed(size);

        // now starts streaming deserialization.
        msgpack::object_handle oh;
        while(pac.next(oh)) {
            std::cout << "<--" << oh.get() << std::endl;
        }

        boost::this_thread::sleep(boost::posix_time::seconds(1));
        write_read();
    } else std::cerr << "read " << ec.message() << std::endl;
}

void connect_handler(boost::system::error_code ec) {
    if (!ec) {
//        std::string str = "abcv}ddd";
//        boost::asio::write(sock, boost::asio::buffer(str));
//        sock.async_read_some(boost::asio::buffer(buffer), read_handler);
        write_read();
    } else std::cerr << "connect " << ec.message() << std::endl;
}

void resolve_handler(boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator it) {
    if (!ec) {
        sock.async_connect(*it, connect_handler);
    } else std::cerr << "resolve " << ec.message() << std::endl;
}

int main() {
    boost::asio::ip::tcp::resolver::query query("127.0.0.1", "7000");
    resolver.async_resolve(query, resolve_handler);
    io_service.run();
}