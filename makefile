CC = gcc
CFLAGS = -w
server: 
	$(CC) -o server src/server.c src/handle_request.c -w

clean:
	rm  server