
#include "config1.h"
ssize_t readline(int fd, char *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char    c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ((rc = read(fd,&c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;
            } else if (rc == 0) {
                *ptr = 0;
                return(n - 1);
            } else
                return(-1);
    }
   *ptr = 0;
   return(n);
}
int main(int argc, char ** argv)
{
    int sockfd;
    int PORT = 80;
    struct sockaddr_in servaddr;
    if ((argc < 2 || argc > 3) == 1) {
        perror("usage:tcpcli <IPaddress> and port");
        exit(1);
    }
   
    if (argc == 3){
        PORT = atoi(argv[2]);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
        printf("inet_pton error for %s\n", argv[1]);
        exit(1);
    }
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        exit(1);
    }
     char sendline[MAX_LINE], recvline[MAX_LINE];
     while(fgets(sendline, MAX_LINE, stdin) != NULL)
     {
         write(sockfd, sendline, strlen(sendline));
         if (readline(sockfd, recvline, MAX_LINE) == 0){
            perror("server terminated prematurely");
            exit(1);
         }
         if (fputs(recvline, stdout) == EOF){
            perror("fputs error");
            exit(1);
         }
     }
     close(sockfd);
}
