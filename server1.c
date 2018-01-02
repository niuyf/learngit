#include "config1.h"
int main(int argc, char **argv)
{
    struct sockaddr_in servaddr, cliaddr;
    int listenfd, connfd;
    pid_t childpid;
    int PORT = 80;
    if (argc == 2) {
        PORT = atoi(argv[1]);
    }

    char buf[MAX_LINE];
    socklen_t clilen;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind error");
        exit(1);
    }
    if (listen(listenfd, LISTENQ) < 0){
        perror("listen error");
        exit(1);
    }
    for ( ; ; )
    {
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
            perror("accept error");
            exit(1);
        }
        if ((childpid = fork()) == 0) {
            close(listenfd);
            ssize_t n;
            char buff[MAX_LINE];
            while((n = read(connfd, buff, MAX_LINE)) > 0)
            {
                write(connfd, buff, n);
            }
            exit(0);
        }
        close(connfd);
    }
    close(listenfd);
}
