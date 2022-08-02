#include "tcp_epoll_serv.h"

int main(int argc, char const *argv[])
{
    if(argc != 3)
    {
        printf("参数错误，正确用法是 ./arv[0]:可执行程序名 argv[1]:服务端端口号 argv[2]:要传输给客户端的文件的绝对路径\n");
        exit(1);
    }
    run(argv[1], argv[2]);
    return 0;
}
