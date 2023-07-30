#include "header.h"

int main() {
  int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd == -1) {
    perror("Error on socket creation: ");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(SOCKET_PORT);
  server.sin_addr.s_addr = inet_addr(IP_ADDR);

  int ret = connect(socket_fd, (const struct sockaddr *)&server, sizeof(struct sockaddr_in));
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

  memset(message, '\0', 256);
  if(recv(socket_fd, message, 256, 0) == -1) {
    perror("Error on client recv message: ");
    exit(EXIT_FAILURE);
  }

  printf("Message from server: %s\n", message);

  close(socket_fd);
  return EXIT_SUCCESS;
}