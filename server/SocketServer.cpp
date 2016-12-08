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
    char data[1024];
    
    error_code error;
    size_t length = socket.get()->read_some(boost::asio::buffer(data), error);

    if (error)
      std::cerr << error << std::endl;

    std::cout << data << length << std::endl;
    return;
    
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
