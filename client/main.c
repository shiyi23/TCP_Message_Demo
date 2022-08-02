#include "tcp_client.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief 客户端进程.
 * 编译命令:gcc -std=c99 -g -Wall adlist.c zmalloc.c tcp_client.c main.c -o client  -lpthread
 * 运行方式:./client 服务端ip 服务端端口号 的方式运行。
 */

volatile BOOL g_is_my_parser_finish = FALSE;

int main(int argc, char const *argv[]) //arv[0]:可执行程序名 argv[1]:服务端ip argv[2]:服务端端口号
{   
    if(argc != 3)
    {
        printf("参数错误， 正确用法是 ./arv[0]:可执行程序名 argv[1]:服务端ip argv[2]:服务端端口号\n");
        exit(EXIT_FAILURE);
    }
    int result = run(argv[1], argv[2]);
    if (result == 1) 
    {
        printf("本程序结束!\n");
        exit(0);
    }
    else if (result == 0)
    {
        // pthread_mutex_t main_lock = PTHREAD_MUTEX_INITIALIZER;
        // pthread_mutex_lock(&main_lock);
        while(!g_is_my_parser_finish) 
        {
            
        }
        // pthread_mutex_unlock(&main_lock);
        printf("本程序结束!\n");
        free(clnt);
        free(g_list_sock);
        exit(0);
    }
    // pthread_mutex_unlock(&main_lock);
    return 0;
}
