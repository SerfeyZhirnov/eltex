#include "header.h"

#define POOL_SIZE 8

pthread_mutex_t mutexes[POOL_SIZE];
typedef struct {
  int index;
  int fd;
} ThreadInfo;

void *ThreadFunc(void *);

int main() {
  pthread_mutexattr_t mutex_attr;
  pthread_mutexattr_init(&mutex_attr);
  for (int i = 0; i < POOL_SIZE; ++i) {
    pthread_mutex_init(mutexes + i, &mutex_attr);
  }

  pthread_t pool[POOL_SIZE];
  pthread_attr_t thread_attr;
  pthread_attr_init(&thread_attr);
  pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

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

  ThreadInfo clients[POOL_SIZE];
  memset((void *)clients, -1, sizeof(ThreadInfo) * POOL_SIZE);
  for (int i = 0; i < POOL_SIZE; ++i) {
    clients[i].index = i;
    pthread_create(&pool[i], &thread_attr, ThreadFunc, (void *)(clients + i));
  }

  while (1) {
    int client = accept(socket_fd, NULL, NULL);
    if (client == -1) {
      perror("Error on accept: ");
      exit(EXIT_FAILURE);
    }

    int accepted = 0;
    while (!accepted) {
      for (int i = 0; i < POOL_SIZE; ++i) {
        if (pthread_mutex_trylock(mutexes + i) == 0) {
          clients[i].fd = client;
          accepted = 1;
          pthread_mutex_unlock(mutexes + i);
          break;
        }
      }
    }
  }

  pthread_attr_destroy(&thread_attr);
  for (int i = 0; i < POOL_SIZE; ++i) {
    pthread_mutex_destroy(mutexes + i);
  }
  pthread_mutexattr_destroy(&mutex_attr);

  return EXIT_SUCCESS;
}

void *ThreadFunc(void *info) {
  int index = (*((ThreadInfo *)info)).index;
  char recv_msg[256], send_msg[256] = "Ok!";

  while (1) {
    if (pthread_mutex_trylock(mutexes + index) == 0) {
      int fd = (*((ThreadInfo *)info)).fd;
      if (fd != -1) {
        printf("%d connected!\n", fd);

        while (1) {
          if (recv(fd, recv_msg, sizeof(recv_msg), 0) == -1) {
            perror("Error recv Message: ");
            exit(EXIT_FAILURE);
          }
          printf("Message from client with fd %d in thread %d: %s\n", fd, index, recv_msg);

          if (strcmp(recv_msg, "!exit") == 0) {
            strcpy(send_msg, "closed");
            send(fd, send_msg, sizeof(send_msg), 0);
            break;
          }

          if (send(fd, send_msg, sizeof(send_msg), 0) == -1) {
            perror("Error recv Message: ");
            exit(EXIT_FAILURE);
          }

          memset(recv_msg, '\0', 256);
          memset(send_msg, '\0', 256);
        }

        printf("%d disconnected\n", fd);
        close(fd);
        (*((ThreadInfo *)info)).fd = -1;
      }
      pthread_mutex_unlock(mutexes + index);
    }
  }

  pthread_exit(NULL);
}
