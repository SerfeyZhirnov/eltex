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

  if (bind(socket_fd, (const struct sockaddr *)&server,
           sizeof(struct sockaddr_un)) == -1) {
    perror("Bind error: ");
    exit(EXIT_FAILURE);
  }

  if (listen(socket_fd, 5) == -1) {
    perror("Error on start waiting connect: ");
    exit(EXIT_FAILURE);
  }

  int client_fd = accept(socket_fd, NULL, NULL);
  if (client_fd == -1) {
    perror("Error on connect client to server: ");
    exit(EXIT_FAILURE);
  }

  char message[256];
  if (recv(client_fd, message, 256, 0) == -1) {
    perror("Error on server recv message: ");
    exit(EXIT_FAILURE);
  }
  printf("Message from client: %s\n", message);

  strcpy(message, "Hi!");
  if (send(client_fd, message, 256, 0) == -1) {
    perror("Erorr on server send message: ");
    exit(EXIT_FAILURE);
  }

  close(client_fd);
  unlink(SOCKET_NAME);
  close(socket_fd);
  return EXIT_SUCCESS;
}
