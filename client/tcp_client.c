#include "tcp_client.h"
#include <pthread.h>

//宏定义布尔类型(兼容C89)
#define BOOL int
#define TRUE 1
#define FALSE 0

void *my_clnt_recv(void *param) //接收文件数据
{
    intptr_t sock_convert;
    list_and_sock *temp_list_sock_obj = (list_and_sock *)param;
    sock_convert = temp_list_sock_obj->param_sock;
    while (1)
    {
        int RB = read(sock_convert, mes_from_server_, sizeof(mes_from_server_));
        if (RB < -1)
        {
            free(clnt);
            free(g_list_sock);
            client_error_handling("read() ERROR!");
        }

        if (RB > 0)
        {
            if (pthread_mutex_init(&stor_list_lock, NULL) != 0) //初始化对于存储链表的锁
            {
                free(clnt);
                free(g_list_sock);
                client_error_handling("Init stor_list_lock failed !");
            }
            pthread_mutex_lock(&stor_list_lock);

            if (listAddNodeTail(temp_list_sock_obj->storage_list_, (void *)mes_from_server_) == NULL) //接收者线程往存储链表末尾添加元素
            {
                free(clnt);
                free(g_list_sock);
                client_error_handling("listAddNodeTail FAILED!");
            }
            pthread_mutex_unlock(&stor_list_lock);

            int err_1;
            //创建 解析者线程
            err_1 = pthread_create(&clnt->ntid_1, NULL, my_parser, (void *)temp_list_sock_obj->storage_list_);
            if (err_1 != 0)
            {
                free(clnt);
                free(g_list_sock);
                client_error_handling("Cant't create my_parser thread!");
            }
        }
        else if (RB == 0)
        {
            printf("read()完全部文件或服务端关闭了!\n");
            break;
        }
    }
    close(sock_convert);
    return NULL;
}

void *my_parser(void *param_list)
{
    list *temp_list = listCreate();
    listIter *iter = listGetIterator(temp_list, AL_START_HEAD);
    temp_list = (list *)param_list;
    if (temp_list->len == 0)
        return NULL;
    iter->next = temp_list->head;
    int count = 0;
    while (count < temp_list->len)
    {
        char *line = iter->next->value;
        printf("文件的内容是:%s \n", line);
        iter->next = listNext(iter);
        count += 1;
    }
    g_is_my_parser_finish = TRUE;
    free(iter);
    return NULL;
}

int run(const char *server_ip, const char *server_port)
{
    if (!server_ip || !server_port)
        client_error_handling("服务端ip及服务端端口号均不能为空!");
    strncpy(mes_from_server_, "\0", sizeof(mes_from_server_));
    strncpy(clnt_pong_, "\0", sizeof(clnt_pong_));
    clnt = NULL;
    g_list_sock = NULL;
    clnt = malloc(sizeof(struct tcp_client));
    if (!clnt)
        client_error_handling("Can't malloc for *clnt");

    clnt->the_clnt_sock_ = socket(PF_INET, SOCK_STREAM, 0);
    if (clnt->the_clnt_sock_ == -1)
    {
        free(clnt);
        client_error_handling("socket() error!");
    }

    memset(&clnt->serv_addr_, 0, sizeof(clnt->serv_addr_));
    clnt->serv_addr_.sin_family = AF_INET;
    clnt->serv_addr_.sin_addr.s_addr = inet_addr(server_ip);
    clnt->serv_addr_.sin_port = htons(atoi(server_port));

    if (connect(clnt->the_clnt_sock_, (struct sockaddr *)&clnt->serv_addr_, sizeof(clnt->serv_addr_)) == -1)
    {
        free(clnt);
        client_error_handling("connect() error!");
    }
    else
        puts("Connected............");

    int input_flag = -1;
    while (1)
    {
        fputs("Input words 'ping' to communicate with the Server or input 'quit' to quit the program, other input will begin receving data from Server.\n", stdout);
        fgets(clnt->client_input_message_, sizeof(clnt->client_input_message_), stdin);
        if (strcmp(clnt->client_input_message_, "quit\n") == 0)
        {
            input_flag = 0;
            break;
        }
        if (strcmp(clnt->client_input_message_, "ping\n") != 0)
        {
            write(clnt->the_clnt_sock_, "save\n", sizeof(clnt->client_input_message_));
            break;
        }
        write(clnt->the_clnt_sock_, clnt->client_input_message_, sizeof(clnt->client_input_message_));
        strncpy(clnt->client_input_message_, "\0", sizeof(clnt->client_input_message_));
        read(clnt->the_clnt_sock_, clnt_pong_, sizeof(clnt_pong_));
        printf("Message from Server is: '%s' \n", clnt_pong_);
    }
    if (input_flag == 0)
    {
        close(clnt->the_clnt_sock_);
        free(clnt);
        return 1;
    }

    g_list_sock = malloc(sizeof(struct list_and_sock));
    if (!g_list_sock)
    {
        free(clnt);
        client_error_handling("Can't malloc for g_list_sock");
    }

    g_list_sock->storage_list_ = listCreate();
    g_list_sock->param_sock = clnt->the_clnt_sock_;
    int err_0 = -1;
    err_0 = pthread_create(&clnt->ntid_0, NULL, my_clnt_recv, (void *)g_list_sock);
    if (err_0 != 0)
    {
        free(clnt);
        free(g_list_sock);
        client_error_handling("Cant't create my_clnt_recv thread!");
    }
    return 0;
}

void client_error_handling(const char *clnt_err_msg)
{
    fputs(clnt_err_msg, stderr);
    fputc('\n', stderr);
    exit(EXIT_FAILURE);
}
