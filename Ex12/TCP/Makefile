C_FLAGS = -Wall -Werror -Wextra

all: server client

server: server.c header.h
	gcc $(C_FLAGS) server.c -o server

client: client.c header.h
	gcc $(C_FLAGS) client.c -o client
	