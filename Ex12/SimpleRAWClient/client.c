#include "header.h"

int main() {
  int socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (socket_fd == -1) {
    perror("Error on socket creation: ");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(SOCKET_PORT);
  server.sin_addr.s_addr = inet_addr(IP_ADDR);

  if (bind(socket_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("Bind error: ");
    exit(EXIT_FAILURE);
  }

  char message[256];
  strcpy(message, "Hello!");

  struct udphdr header;
  header.source = ntohs(7777);
  header.dest = ntohs(SOCKET_PORT);
  header.len = ntohs(sizeof(struct udphdr) + strlen(message));
  header.check = 0;

  char buffer[256];
  memcpy(buffer, (const void *)&header, sizeof(header));
  memcpy(buffer + sizeof(header), message, strlen(message));

  if (sendto(socket_fd, (void *)buffer, sizeof(struct udphdr) + strlen(message),
             0, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("Send error: ");
    exit(EXIT_FAILURE);
  }

  close(socket_fd);
  return EXIT_SUCCESS;
}