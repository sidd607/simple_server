
/*
    author: siddartha padhi
    email: siddartha.padhi@gmail.com
    last modified: 3 October 2017
*/

#define BUFSIZE 8096
#define ERROR 42
#define SORRY 43
#define LOG   44
#include <time.h>

void log(int type, char *s1, char *s2, int num)
{
	int fd ;
	char logbuffer[BUFSIZE*2];
    printf("log: %s %s \n", s1, s2);

	switch (type) {
	case ERROR: sprintf(logbuffer,"ERROR: %s:%s Errno=%d exiting pid=%d",s1, s2, errno,getpid()); break;
	case SORRY: 
		sprintf(logbuffer, "<HTML><BODY><H1>Web Server Sorry: %s %s</H1></BODY></HTML>\r\n", s1, s2);
		write(num,logbuffer,strlen(logbuffer));
		sprintf(logbuffer,"SORRY: %s:%s",s1, s2); 
		break;
	case LOG: sprintf(logbuffer," INFO: %s:%s:%d",s1, s2,num); break;
	}	
	
	if((fd = open("server.log", O_CREAT| O_WRONLY | O_APPEND,0644)) >= 0) {
		write(fd,logbuffer,strlen(logbuffer)); 
		write(fd,"\n",1);      
		close(fd);
	}
	if(type == ERROR || type == SORRY) exit(3);
}

void weblog(int type, char* request, int status, char *ip, char *hostname){
    int fd;
    time_t timer;
    char logbuffer[1024];
    char time_buffer[26];
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(time_buffer, 26, "%d/%m/%Y:%H:%M:%S", tm_info);
    sprintf(logbuffer, "%s - %s [%s] '%s' %d\n",ip, hostname, time_buffer, request, status);
    printf("%s", logbuffer);
    if((fd = open("server.log", O_CREAT| O_WRONLY | O_APPEND,0644)) >= 0) {
        write(fd, logbuffer, strlen(logbuffer));
        close(fd);
    }
    
}