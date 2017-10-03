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
#include "handle_request.h"

struct {
	char *ext;
	char *filetype;
} extensions [] = {
	{"gif", "image/gif" },  
	{"jpg", "image/jpeg"}, 
	{"jpeg","image/jpeg"},
	{"png", "image/png" },  
	{"zip", "image/zip" },  
	{"gz",  "image/gz"  },  
	{"tar", "image/tar" },  
	{"htm", "text/html" },  
	{"html","text/html" },  
	{"php", "image/php" },  
	{"cgi", "text/cgi"  },  
	{"asp","text/asp"   },  
	{"jsp", "image/jsp" },  
	{"xml", "text/xml"  },  
	{"js","text/js"     },
   {"css","test/css"   }, 
	{0,0} };


int handle_get(int client, char* request){
    /*
        Handles get methid as specified in the request
        Extracts File name from the request, if file exists
        sends the file to client else sends 404

        returns the status code.
    */


    //print("handling request: %s", request );
    int i, flag = 0, file_fd, ret;
    char *file_path, *fstr;
    static char buffer[1025];
    for(i=4; i<strlen(request); i++){
        if (request[i] == ' '){
            request[i] = 0;
            //print("\nNew request: %s\n", request);
            break;
        }
    }
    file_path = request + 4;
    if(strcmp(file_path, "/")==0){
        strcpy(file_path, "index.html");
    }
    else{
        file_path = file_path + 1;
    }
    //print("\nfile path: %s", file_path);
    
    file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1){
        //print("404 File not found");
        send_404(client);
        return 404;
    }
    else{
        /*
        int buflen=strlen(file_path);
        char *fstr = (char *)0;
        int len;
        for(i=0;extensions[i].ext != 0;i++) {
            len = strlen(extensions[i].ext);
            if( !strncmp(&file_path[buflen-len], extensions[i].ext, len)) {
                fstr =extensions[i].filetype;
                break;
            }
        }
        printf("->%s<-", fstr); // content type
        */
        // I can extract the file name and the corresponding file type but when adding 
        // it to the buffer it thors an error
        // sprintf(buffer, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", fstr); fails

        sprintf(buffer, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", "text/html");
        write(client, buffer, strlen(buffer));

        while( (ret=read(file_fd, buffer, 1024))>0){
            write(client, buffer, ret);
        }
    }
    close(file_path);
    return 200;
}

int handle_head(int client, char* request){

    /*
        Function similar to handle_get
        instead of sending the file data, it sends just the header.

        returns the status code.
    */


    //print("handling request: %s", request );
    int i, flag = 0, file_fd, ret;
    char *file_path;
    static char buffer[1025];
    for(i=5; i<strlen(request); i++){
        if (request[i] == ' '){
            request[i] = 0;
            //print("\nNew request: %s\n", request);
            break;
        }
    }
    file_path = request + 4;
    if(strcmp(file_path, "/")==0){
        strcpy(file_path, "index.html");
    }
    else{
        file_path = file_path + 1;
    }
    //print("\nfile path: %s", file_path);
    
    file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1){
        //print("404 File not found");
        send_404(client);
        return 404;
    }
    else{
        //print("200 OK");
        sprintf(buffer, "HTTP/1.1 200 OK\r\n\
                         Connection: close\r\n\r\n");
        write(client, buffer, strlen(buffer));
    }
    close(file_path);  
    return 200;  
}

int handle_post(int client, char *request){

    /*
        Right now the methods extracts the data from the 
        header and prints the data.
    */


    int i;
    char* data, *message;
    data = strstr(request, "\r\n\r\n");
    if (data != NULL){
        data = data+4;
        printf("data recieved from post: %s\n", data);
    }
    message =   "HTTP/1.1 200 OK\r\n\
                 Connection: close\r\n\
                 Content-Type: text/html; charset=utf-8\r\n\r\n\
                 {'data': 'accepted'}";
    write(client, message, strlen(message));
    return 200;
}
int handle_put(int client, char* request){
    handle_post(client, request);
    return 200;
}

int handle_options(int client, char *request){

    /*
        Retuns the options allowed i.e
        GET, HEAD, OPTIONS
    */

    char *message = "HTTP/1.1 200 OK\nAllows: OPTIONS, GET, HEAD\n\n" ;
    write(client, message, strlen(message));
    return 200;
}

void send_404(int client){

    /*
        Sends a 404 response code to client along with 
        data.
    */

    char *message ="HTTP/1.1 404 Not Found\r\n\
                    Connection: close\r\n\
                    Content-Type: text/html; charset=utf-8\r\n\r\n\
                    <html><body><center><h2>404 - Page not found </h2></center></body></html>";
    write(client, message, strlen(message));
}
void handle_bad_request(int client){
    char *message = "HTTP/1.1 400 Bad Request\r\n\
                     Connection: close\r\n\r\n";
    write(client, message, strlen(message));
}