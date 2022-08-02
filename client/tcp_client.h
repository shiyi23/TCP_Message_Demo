#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include "adlist.h"

//宏定义布尔类型(兼容C89)
typedef int BOOL;
#define TRUE 1
#define FALSE 0

extern volatile BOOL g_is_my_parser_finish;

pthread_mutex_t stor_list_lock;

typedef struct tcp_client
{
    // int the_clnt_sock_;
    intptr_t the_clnt_sock_;
    struct sockaddr_in serv_addr_;
    char client_input_message_[1024];
    long THE_LEN_;
    pthread_t ntid_0;
    pthread_t ntid_1;
} tcp_client;

tcp_client *clnt;

typedef struct list_and_sock
{
    intptr_t param_sock;
    list *storage_list_;
} list_and_sock;

list_and_sock *g_list_sock;

void* my_clnt_recv(void* param);//接收文件数据

void* my_parser(void* param_list);//解析、打印文件数据
char mes_from_server_[4096];
char clnt_pong_[20];

int run(const char* server_ip, const char* server_port);
void client_error_handling(const char* message);

#endif /*__TCP_CLIENT_H__ */
