#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define CLNT_INPUT_BUF_SIZE 1024
#define BUF_SIZE 256
#define EPOLL_SIZE 100

FILE* fp_open_;

void server_error_handling(const char* message);

void run(const char* port, const char* file_name);