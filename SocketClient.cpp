//
// Created by zll on 16-8-11.
//

#include "SocketClient.h"

SocketClient::SocketClient( shared_ptr<tcp::socket> socket) : socket_(socket) {

}

void SocketClient::write_handler(error_code ec, size_t bytes_transferred)
{
}

void SocketClient::Write( string msg) {
    this->socket_->async_write_some(buffer(msg), bind(&SocketClient::write_handler, this, _1, _2));
}
