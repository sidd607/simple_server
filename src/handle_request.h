
/*
    author: siddartha padhi
    email: siddartha.padhi@gmail.com
    last modified: 3 October 2017
*/

int handle_get(int, char*);
int handle_head(int, char*);
int handle_post(int, char*);
int handle_put(int, char*);
int handle_trace(int, char*);
int handle_options(int, char*);
int handle_connect(int, char*);
void handle_bad_request(int);
void send_404(int);
