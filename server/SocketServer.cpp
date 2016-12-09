#include "SocketServer.h"

SocketServer::SocketServer() :
    service_(),
    acceptor_(service_,  tcp::endpoint(tcp::v4(), 7000))//, handles({{"join", join}})
{
    handles["join"] = &SocketServer::join;// = {{"join", join}};
}

void SocketServer::listen() {
    accept();
}

void SocketServer::accept() {
    std::shared_ptr< tcp::socket> psocket(new tcp::socket(service_));

    acceptor_.async_accept(*psocket, [psocket, this](error_code ec){
        if (ec) {
            std::cerr << "async_accept " << ec.message() <<std::endl;
            return;
        }
        this -> process(psocket);
        this -> accept();
    });
    service_.run();
}

void SocketServer::process(std::shared_ptr<tcp::socket> socket)
{
    using namespace boost::asio::error;

    streambuf buffer;
    async_read_until(*socket, buffer, '}', [](error_code ec, size_t n) {
        std::cout << "xxx" << ec << n << std::endl;
    });

    return;

//    async_read_until(*socket, *buffer, '\0', [buffer, this, socket](error_code ec, std::size_t n) {
//        if (ec) {
//            std::cerr << "async_read_until " << ec.message() <<std::endl;
//            return;
//        }
//
//        streambuf::const_buffers_type bufs = buffer->data();
//        std::string line( buffers_begin(bufs),  buffers_begin(bufs) + n-1);
//        std::cout<< "xxx" << line <<std::endl;
//        this -> operate(line);
//        this -> process(socket);
//    });
    
}

void SocketServer::operate(std::string msg) {

    try{
        std::stringstream ss(msg);
        ptree pt;
        read_json(ss, pt);

        std::string code = pt.get<std::string>("code");

        auto handle = handles[code];
        if (handle != nullptr) {
            ptree params = pt.get_child("params");  // get_child得到数组对象
            (this ->* handle)(params);
        } else {
            std::cerr << "unknown code "  <<std::endl;
        }
//        ptree image_array = pt.get_child("images");  // get_child得到数组对象
//
//        std::for_each(image_array.begin(), image_array.end(), [](boost::property_tree::ptree::value_type &v){
//
//            std::stringstream s;
//            write_json(s, v.second);
//            std::string image_item = s.str();
//            std::cerr << "get_child " <<image_item <<std::endl;
//        });
    }
    catch (ptree_error & pe)
    {
        std::cerr << "operate " <<std::endl;
        return ;
    }
}

void SocketServer::join(ptree pt) {
    std::cout << "operate " << pt.get<std::string>("name") <<std::endl;
}
