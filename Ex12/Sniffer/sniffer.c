#include "header.h"

int main() {
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(SOCKET_PORT);
  server.sin_addr.s_addr = inet_addr(IP_ADDR);

  int socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (socket_fd == -1) {
    perror("Socket: ");
    exit(EXIT_FAILURE);
  }

  if (bind(socket_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("Bind: ");
    exit(EXIT_FAILURE);
  }

  char packet[256];
  int count = 0;
  while(1) {
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    if (recvfrom(socket_fd, packet, 256, 0, (struct sockaddr *)&client, &len) == -1) {
      perror("Recv: ");
      exit(EXIT_FAILURE);
    }
    printf("Packet count: %d\n", ++count);
  }

  return EXIT_SUCCESS;
}
