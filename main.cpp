

#include "SocketServer.h"

int main(int argc, char* argv[])
{
    SocketServer sev;

    //开始侦听socket的连接；和开始接收远程数据
    sev.listen();

    return 0;
}