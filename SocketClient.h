//
// Created by zll on 16-8-11.
//

#ifndef CPPDEMO_SOCKETCLIENT_H
#define CPPDEMO_SOCKETCLIENT_H

#include <boost/asio.hpp>

using namespace boost::asio;
using boost::system::error_code;
using ip::tcp;

using namespace std;
using namespace std::placeholders;

class SocketClient {
private:
     shared_ptr<tcp::socket> socket_;
    void write_handler(error_code ec, size_t bytes_transferred);
public:
    SocketClient( shared_ptr<tcp::socket> socket);
    void Write( string msg);
};


#endif //CPPDEMO_SOCKETCLIENT_H
