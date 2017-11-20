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


void write(std::string str) {

    uint8_t * buff = new uint8_t [8] ;
    buff[0] = 100;
    buff[1] = 126;

    //std::string str = "abc";

    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> pk(&sbuf);
    pk.pack(128);
    pk.pack(str);

    auto buf = boost::asio::buffer(sbuf.data(), sbuf.size());
    std::cout << "size " << sbuf.size() << std::endl;

    uint32_t length = (uint32_t)sbuf.size();
    uint32_t stream = 769;
    buff[2] = (uint8_t)(length >> 16);
    buff[3] = (uint8_t)(length >> 8);
    buff[4] = (uint8_t)(length);
    buff[5] = (uint8_t)(stream >> 16);
    buff[6] = (uint8_t)(stream >> 8);
    buff[7] = (uint8_t)(stream);

    boost::asio::write(sock, boost::asio::buffer(buff, 8));

    boost::asio::write(sock, buf);

    //boost::this_thread::sleep(boost::posix_time::seconds(300));
//return;
}

void waitMsg() {
    while(1) {
        std::string msg;
        std::cin >> msg;
        if (msg == "q") {
            break;
        }
        //std::cout << msg << std::endl;
        write(msg);
    }
}

void connect_handler(boost::system::error_code ec) {
    if (!ec) {
        //write();
        waitMsg();
    } else std::cerr << "connect " << ec.message() << std::endl;
}

void resolve_handler(boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator it) {
    if (!ec) {
        sock.async_connect(*it, connect_handler);
    } else std::cerr << "resolve " << ec.message() << std::endl;
}

int main() {
//    waitMsg();
//    return 0;
    boost::asio::ip::tcp::resolver::query query("127.0.0.1", "7000");
    resolver.async_resolve(query, resolve_handler);
    io_service.run();
}