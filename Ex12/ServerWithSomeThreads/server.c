#include "header.h"

void *ThreadFunc(void *);

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

  if (bind(socket_fd, (const struct sockaddr *)&server,
           sizeof(struct sockaddr_in)) == -1) {
    perror("Bind error: ");
    exit(EXIT_FAILURE);
  }

  if (listen(socket_fd, 5) == -1) {
    perror("Error on start waiting connect: ");
    exit(EXIT_FAILURE);
  }

  int client_fds[1000];
  int i = 0;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  while (1) {
    int client_fd = accept(socket_fd, NULL, NULL);
    if (client_fd == -1) {
      perror("Error on accept: ");
      exit(EXIT_FAILURE);
    }

    client_fds[i] = client_fd;

    printf("Client %d started with fd %d\n", i, client_fd);

    pthread_t pid;
    if (pthread_create(&pid, (const pthread_attr_t *)&attr, &ThreadFunc,
                       (void *)&client_fds[i])) {
      perror("Create thread: ");
      exit(EXIT_FAILURE);
    }

    ++i;
    if (i >= 1000) {
      i = 0;
    }
  }

  close(socket_fd);
  return EXIT_SUCCESS;
}

void *ThreadFunc(void *fd) {
  int client_fd = *((int *)fd);
  char recv_msg[256], send_msg[256] = "Ok!";

  while (1) {
    if (recv(client_fd, recv_msg, sizeof(recv_msg), 0) == -1) {
      perror("Recv message: ");
      pthread_exit(NULL);
    }
    printf("Message from client with fd %d: %s\n", client_fd, recv_msg);

    if (strcmp(recv_msg, "!exit") == 0) {
      strcpy(send_msg, "closed");
      send(client_fd, send_msg, sizeof(send_msg), 0);
      break;
    }

    if (send(client_fd, send_msg, sizeof(send_msg), 0) == -1) {
      perror("Recv message: ");
      pthread_exit(NULL);
    }

    memset(recv_msg, '\0', 256);
    memset(send_msg, '\0', 256);
  }

  printf("Connection with fd %d closed\n", client_fd);
  pthread_exit(fd);
}
