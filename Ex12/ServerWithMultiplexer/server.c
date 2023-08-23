#include "header.h"

int create_socket(int);

int main() {
  int socket_udp_fd = create_socket(SOCK_DGRAM);
  int socket_tcp_fd = create_socket(SOCK_STREAM);
  if (listen(socket_tcp_fd, 5) == -1) {
    perror("Listen error: ");
    exit(EXIT_FAILURE);
  }

  fd_set socket_fds;
  FD_ZERO(&socket_fds);
  const int maxfd =
      (socket_udp_fd > socket_tcp_fd) ? socket_udp_fd : socket_tcp_fd;

  char message[256];
  while(1) {
    FD_SET(socket_udp_fd, &socket_fds);
    FD_SET(socket_tcp_fd, &socket_fds);

    if (select(maxfd + 1, &socket_fds, NULL, NULL, NULL) == -1) {
      perror("Select error: ");
      exit(EXIT_FAILURE);
    }

    if (FD_ISSET(socket_udp_fd, &socket_fds)) {
      if (recvfrom(socket_udp_fd, (void *)message, sizeof(message), 0, NULL, NULL) == -1) {
        perror("Recv error: ");
        exit(EXIT_FAILURE);
      }
      printf("Message from UDP: %s\n", message);
    }

    if (FD_ISSET(socket_tcp_fd, &socket_fds)) {
      int client_fd = accept(socket_tcp_fd, NULL, NULL);
      if (client_fd == -1) {
        perror("Accept error: ");
        exit(EXIT_FAILURE);
      }

      if (recv(client_fd, (void *)message, 256, 0) == -1) {
        perror("Recv error: ");
        exit(EXIT_FAILURE);
      }

      printf("Message from TCP: %s\n", message);
      close(client_fd);
    }
  }

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
