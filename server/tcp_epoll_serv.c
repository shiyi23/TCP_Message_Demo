#include "tcp_epoll_serv.h"

void server_error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(EXIT_FAILURE);
}

void run(const char *port, const char *file_name)
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t adr_sz;

    int i;
    char buf[BUF_SIZE];

    struct epoll_event *ep_events;
    struct epoll_event my_event;
    int epfd, event_cnt;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(port));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        server_error_handling("bind() error");

    if (listen(serv_sock, 5) == -1) //设置等待队列的size是5
        server_error_handling("listen() error");

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    my_event.events = EPOLLIN;
    my_event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &my_event);

    while (1)
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, 5000); //超时时间设置为5秒
        if (event_cnt == -1)
        {
            puts("epoll_wait() error");
            break;
        }
        if (event_cnt == 0)
            printf("服务端等待客户端的连接中......\n");
        for (i = 0; i < event_cnt; i++)
        {
            fp_open_ = fopen(file_name, "r"); //以只读方式打开普通文本文件
            if (!fp_open_)
                server_error_handling("open_file() error.");
            printf("成功打开文件!\n");

            if (ep_events[i].data.fd == serv_sock)
            {
                adr_sz = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &adr_sz);
                my_event.events = EPOLLIN;
                my_event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &my_event);
                printf("connected client: %d \n", clnt_sock);
            }
            else
            {
                while (read(ep_events[i].data.fd, buf, BUF_SIZE) != 0)
                {
                    while (strcmp(buf, "ping\n") == 0)
                    {
                        printf("Message from Client is 'ping'\n");
                        char PONG[20] = "pong";
                        write(ep_events[i].data.fd, PONG, sizeof(PONG));
                        strncpy(PONG, "\0", sizeof(PONG));
                        break;
                    }
                    if (strcmp(buf, "save\n") == 0)
                        break;
                }
                printf("Start transferring the contents of the file to the Client...... \n");
                while (feof(fp_open_) == 0) //没到文件末尾就会返回0，到了文件末尾会返回非零值
                {
                    char file_arrar[4096];
                    fread(file_arrar, sizeof(file_arrar[0]), sizeof(file_arrar) / sizeof(file_arrar[0]), fp_open_);
                    write(ep_events[i].data.fd, file_arrar, sizeof(file_arrar));
                }
                printf("File tranfer finished! Server will close the socket with the Client soon......\n");
                close(clnt_sock);
            }
        }
    }
    fclose(fp_open_);
    close(serv_sock);
    close(epfd);
}