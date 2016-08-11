

#include "SocketServer.h"

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