#include "header.h"

int create_socket(int);

int main() {
  int socket_udp_fd = create_socket(SOCK_DGRAM);
  int socket_tcp_fd = create_socket(SOCK_STREAM);

  

  close(socket_tcp_fd);
  close(socket_udp_fd);
  return EXIT_SUCCESS;
}

int create_socket(int type) {
  int socket_fd = socket(AF_INET, type, 0);
  if (socket_fd == -1) {
    perror("Error on socket creation: ");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(SOCKET_PORT);
  server.sin_addr.s_addr = inet_addr(IP_ADDR);

  if (bind(socket_fd, (const struct sockaddr *)&server,
           sizeof(struct sockaddr_in)) == -1) {
    perror("Bind error: ");
    exit(EXIT_FAILURE);
  }
  
  return socket_fd;
}
