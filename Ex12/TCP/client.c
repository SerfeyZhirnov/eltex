#include "header.h"

int main() {
  int socket_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    perror("Error on socket creation: ");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_un server;
  server.sun_family = AF_LOCAL;
  strncpy(server.sun_path, SOCKET_NAME, sizeof(server.sun_path) - 1);

  int ret = connect(socket_fd, (const struct sockaddr *)&server,
                    sizeof(struct sockaddr_un));
  if (ret == -1) {
    perror("Error on connect server to client: ");
    exit(EXIT_FAILURE);
  }

  char message[256];
  strcpy(message, "Hello!");
  if (send(socket_fd, message, 256, 0) == -1) {
    perror("Erorr on client send message: ");
    exit(EXIT_FAILURE);
  }

  if (recv(socket_fd, message, 256, 0) == -1) {
    perror("Error on client recv message: ");
    exit(EXIT_FAILURE);
  }

  printf("Message from server: %s\n", message);

  close(socket_fd);
  return EXIT_SUCCESS;
}
