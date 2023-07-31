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

  char message[256];
  while (1) {
    printf("Please, enter message:");
    fgets(message, 256, stdin);
    if (send(socket_fd, message, strlen(message) - 1, 0) == -1) {
      perror("Error on send: ");
      exit(EXIT_FAILURE);
    }

    if (recv(socket_fd, message, sizeof(message), 0) == -1) {
      perror("Error on recv: ");
      exit(EXIT_FAILURE);
    }

    if (strcmp(message, "!exit") == 0) {
      break;
    }
  }

  close(socket_fd);
  return EXIT_SUCCESS;
}
