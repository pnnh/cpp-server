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

    uint8_t * buff = new uint8_t [8] ;
    buff[0] = 100;
    buff[1] = 126;
//    buff[3] = 4;
//    buff[4] = 5;
//    char * buff = new char [5] ;
//    buff[0] = 'a';
//    buff[1] = 'b';
//    buff[2] = 'c';
//    buff[3] = 'd';
//    buff[4] = 'e';



    std::string str = "abc";

    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> pk(&sbuf);
    pk.pack(128);
    pk.pack(std::string("Log message ... " + std::to_string(n++)));
    pk.pack(std::string("=================="));

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


//    boost::asio::write(sock, boost::asio::buffer(buff, 1));
//    boost::this_thread::sleep(boost::posix_time::seconds(2));
//    boost::asio::write(sock, boost::asio::buffer(buff, 3));
//    boost::this_thread::sleep(boost::posix_time::seconds(2));
//    boost::asio::write(sock, boost::asio::buffer(buff, 4));
//    boost::this_thread::sleep(boost::posix_time::seconds(2));
//    boost::asio::write(sock, boost::asio::buffer(buff, 2));
//    boost::this_thread::sleep(boost::posix_time::seconds(2));
//    boost::asio::write(sock, boost::asio::buffer(buff, 5));

    boost::this_thread::sleep(boost::posix_time::seconds(300));
return;

//    std::string str = "abc";
//
//    msgpack::sbuffer sbuf;
//    msgpack::packer<msgpack::sbuffer> pk(&sbuf);
//    pk.pack(128);
//    pk.pack(std::string("Log message ... " + std::to_string(n++)));
//    pk.pack(std::string("=================="));
//
//    auto buf = boost::asio::buffer(sbuf.data(), sbuf.size());
//    std::cout << "size " << sbuf.size() << std::endl;
//
//
//    boost::asio::write(sock, buf);

//    int n = 0;
//    while(n++ < 3) {
//        std::string str = "hello " + std::to_string(n) + "  ";
//        boost::asio::write(sock, boost::asio::buffer(str));
//    }

    //sock.async_read_some(boost::asio::buffer(buffer), read_handler);
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