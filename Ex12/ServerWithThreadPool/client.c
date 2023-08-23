#include "header.h"

int main() {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    perror("Error on socket creation: ");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(SOCKET_PORT);
  server.sin_addr.s_addr = inet_addr(IP_ADDR);

  int ret = connect(socket_fd, (const struct sockaddr *)&server,
                    sizeof(struct sockaddr_in));
  if (ret == -1) {
    perror("Error on connect server to client: ");
    exit(EXIT_FAILURE);
  }

  char recv_msg[256], send_msg[256];
  while (1) {
    printf("Please, enter message: ");
    fgets(send_msg, 256, stdin);
    if (send(socket_fd, send_msg, strlen(send_msg) - 1, 0) == -1) {
      perror("Error on send: ");
      exit(EXIT_FAILURE);
    }

    if (recv(socket_fd, recv_msg, sizeof(recv_msg), 0) == -1) {
      perror("Error on recv: ");
      exit(EXIT_FAILURE);
    }

    if (strcmp(recv_msg, "closed") == 0) {
      break;
    }

    memset(recv_msg, '\0', 256);
    memset(send_msg, '\0', 256);
  }

  close(socket_fd);
  return EXIT_SUCCESS;
}
