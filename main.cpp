

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

using namespace boost::asio;

using boost::system::error_code;
using ip::tcp;

struct CHelloWorld_Service
{
    //类的初始化创建：设置io_service, 设置端口
    CHelloWorld_Service(io_service &iosev)
            :m_iosev(iosev),m_acceptor(iosev, tcp::endpoint(tcp::v4(), 7000))
    {
    }

    //创建一个tcp的socket；且还是侦听
    void start()
    {
        // 开始等待连接（非阻塞）
        boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));

        // 触发的事件只有error_code参数，所以用boost::bind把socket绑定进去
        m_acceptor.async_accept(*psocket, boost::bind(&CHelloWorld_Service::accept_handler, this, psocket, _1) );
    }

    // 有客户端连接时accept_handler触发
    void accept_handler(boost::shared_ptr<tcp::socket> psocket, error_code ec)
    {
        if(ec) return;

        // 继续等待连接
        start();

        // 显示远程IP
        std::cout << psocket->remote_endpoint().address() << std::endl;

        // 发送信息(非阻塞)
        boost::shared_ptr<std::string> pstr(new std::string("hello async world!"));
        psocket->async_write_some(buffer(*pstr),
                                  boost::bind(&CHelloWorld_Service::write_handler, this, pstr, _1, _2)
        );
        boost::shared_ptr<std::string> pstr2(new std::string(" world!"));

        //psocket->async_read_some(buffer(data_, 256), boost::bind(&CHelloWorld_Service::read_handler, this, psocket, data_ , _1, _2));
        read_handler(psocket, data_, error_code(), 0);
    }

    // 异步写操作完成后write_handler触发
    void write_handler(boost::shared_ptr<std::string> pstr, error_code ec, size_t bytes_transferred)
    {
        if(ec)
            std::cout<< "发送失败!" << std::endl;
        else
            std::cout<< *pstr << " 已发送" << std::endl;
    }

    void read_handler(boost::shared_ptr<tcp::socket> psocket, char* data,  error_code ec, size_t s)
    {
        psocket->async_read_some(buffer(data_, 256), boost::bind(&CHelloWorld_Service::read_handler, this, psocket, data_ , _1, _2));

        std::cout <<data<< s<< std::endl;
    }

private:
    char data_[256];
    io_service &m_iosev;
    ip::tcp::acceptor m_acceptor;
};


int main(int argc, char* argv[])
{
    //建立io服务器
    io_service iosev;

    CHelloWorld_Service sev(iosev);

    //开始侦听socket的连接；和开始接收远程数据
    sev.start();

    //开始执行回调函数
    iosev.run();

    return 0;
}