#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main() {
    try
    {
        boost::asio::io_service io_service;

        tcp::socket s(io_service);
        tcp::resolver resolver(io_service);
        boost::asio::connect(s, resolver.resolve({"localhost", "7000"}));

        std::string request("{\"code\":\"join\"}\0");

        boost::asio::write(s, boost::asio::buffer(request, request.length()));

        char reply[max_length];
        size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, request.length()));

        std::cout << "Reply is: ";
        std::cout.write(reply, reply_length);
        std::cout << "\n";
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
