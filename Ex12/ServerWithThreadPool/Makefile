C_FLAGS = -Wall -Werror -Wextra -lpthread

all: server client

server: server.c header.h
	gcc $(C_FLAGS) server.c -o server

client: client.c header.h
	gcc $(C_FLAGS) client.c -o client
	
clean:
	rm client server