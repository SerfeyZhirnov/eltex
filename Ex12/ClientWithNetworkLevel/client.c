#include "header.h"

int main() {
  int socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (socket_fd == -1) {
    perror("Error on socket creation: ");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(CLIENT_PORT);
  server.sin_addr.s_addr = inet_addr(IP_ADDR);

  if (bind(socket_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("Bind error: ");
    exit(EXIT_FAILURE);
  }

  int optval = 1;
  if (setsockopt(socket_fd, IPPROTO_IP, IP_HDRINCL, (void *)&optval,
                 sizeof(optval)) == -1) {
    perror("Setsockopt error: ");
    exit(EXIT_FAILURE);
  }

  char packet[256];
  struct iphdr *network = (struct iphdr *)packet;
  struct udphdr *transport = (struct udphdr *)(packet + sizeof(struct iphdr));
  char *payload = packet + sizeof(struct iphdr) + sizeof(struct udphdr);
  strcpy(payload, "Hello!");

  network->version = IPVERSION;
  network->ihl = 5;
  network->tos = 0;
  network->tot_len = 0;
  network->id = 0;
  network->ttl = 0;
  network->protocol = IPPROTO_UDP;
  network->saddr = inet_addr(IP_ADDR);
  network->daddr = inet_addr(IP_ADDR);

  transport->source = htons(CLIENT_PORT);
  transport->dest = htons(SERVER_PORT);
  transport->len = htons(sizeof(struct udphdr) + strlen(payload));
  transport->check = 0;

  if (sendto(socket_fd, (void *)packet, strlen(packet), 0,
             (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("Send error: ");
    exit(EXIT_FAILURE);
  }

  close(socket_fd);
  return EXIT_SUCCESS;
}