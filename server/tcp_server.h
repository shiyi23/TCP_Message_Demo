#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CLNT_INPUT_BUF_SIZE 1024

class tcp_server
{
private:
    int serv_sock;
    int clnt_sock;
    char clnt_message[CLNT_INPUT_BUF_SIZE];
    char file_arrar[4096];
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    FILE* fp_open_ = NULL;
public:
    tcp_server(const char* port, const char* file_name, int clnt_count);//port: 服务端端口号  file_name: 指定要读取的目标文件名  clnt_count: 要与该服务器端连接的客户端个数
    ~tcp_server();
    void server_error_handling(const char* message);
};

