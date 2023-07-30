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

  if (bind(socket_fd, (const struct sockaddr *)&server,
           sizeof(struct sockaddr_in)) == -1) {
    perror("Bind error: ");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in client;
  char message[256];
  socklen_t structsize = sizeof(IP_ADDR);
  if (recvfrom(socket_fd, (void *)message, sizeof(message), 0,
               (struct sockaddr *)&client, (socklen_t *)&structsize) == -1) {
    perror("Recv error: ");
    exit(EXIT_FAILURE);
  }
  printf("Message from client: %s\n", message);

  strcpy(message, "Hi!");
  if (sendto(socket_fd, (void *)message, strlen(message), 0,
             (const struct sockaddr *)&client, structsize) == -1) {
    perror("Send error: ");
    exit(EXIT_FAILURE);
  }

  close(socket_fd);
  return EXIT_SUCCESS;
}
