/*
    author: siddartha padhi
    email: siddartha.padhi@gmail.com
    last modified: 3 October 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log.c"
#include "handle_request.h"

#define BUFSIZE 8096


void web(int fd, int hit)
{
    // Getting the client's ip address
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(fd, (struct sockaddr *)&addr, &addr_size);
    char clientip[20] ;
    strcpy(clientip, inet_ntoa(addr.sin_addr));
    printf("%s ", clientip);

	int j, file_fd, buflen, len, result;
	long i, ret;
	char * fstr, method[10], *context, delem[] = " ", tmp;
	static char buffer[BUFSIZE+1];

	ret =read(fd,buffer,BUFSIZE); 
	if(ret == 0 || ret == -1) {
		printf("Failed to read browser request");
        exit(1);
	}
	if(ret > 0 && ret < BUFSIZE)	
		buffer[ret]=0;	
	else buffer[0]=0;


	
    
    for (i=0; i<10; i++){
        if (buffer[i] == ' ')
            break;
        method[i] = buffer[i];
    }
    
    if(strcmp(method, "GET" )==0 || strcmp(method, "get" ) == 0){
        result = handle_get(fd, buffer);
    }
    else if (strcmp(method, "HEAD" ) == 0 || strcmp(method,"head" ) == 0){
        result = handle_head(fd, buffer);
    }
    else if (strcmp(method, "PUT" )==0 || strcmp(method, "put" ) == 0){
        result = handle_put(fd, buffer);
    }
    else if (strcmp(method, "POST" )==0 || strcmp(method, "post" ) == 0){
        result = handle_post(fd, buffer);
    }
    else if (strcmp(method, "OPTIONS" )==0 || strcmp(method, "OPTIONS" ) == 0){
        result = handle_options(fd, buffer);
    }
    else {
        handle_bad_request(fd);
        result = 400;
    }
    weblog(1,   buffer, result, clientip, "-");
#ifdef LINUX
	sleep(1);
#endif
	exit(1);
}


int main(int argc, char **argv)
{
	int i, port, pid, listenfd, socketfd, hit;
	size_t length;
	static struct sockaddr_in cli_addr; 
	static struct sockaddr_in serv_addr;
    
    if (argc != 2){
        printf("Usage ./server [PORT]\n");
        exit(0);
    }

	if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0){
        printf("Error creating socket\n");
        exit(1);
    }
	port = atoi(argv[1]);
	if(port < 0 || port >60000){
        printf("Port number out of range\n");
        exit(1);
    }
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0){
        printf("Bind error\n");
        exit(1);
    }
	if( listen(listenfd,64) <0){
        printf("Listen error \n");
        exit(1);
    }
    printf("Server listening @ %d ... \n", port);
	for(hit=1; ;hit++) {
		length = sizeof(cli_addr);
		if((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0){
            printf("Accept error\n");
            exit(1);
        }
		if((pid = fork()) < 0) {
			printf("Fork, error\n");
            exit(1);
		}
		else {
			if(pid == 0) {
				(void)close(listenfd);
				web(socketfd,hit);
			} else {
				(void)close(socketfd);
			}
		}

	}
}