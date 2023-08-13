C_FLAGS = -Wall -Werror -Wextra

all: sniffer

sniffer: sniffer.c header.h
	gcc $(C_FLAGS) sniffer.c -o sniffer

clean:
	rm sniffer