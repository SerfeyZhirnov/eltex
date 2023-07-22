#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MSG_SIZE 256
#define MAX_MSG 5

int main() {
  const char name[] = "/queue", message[MSG_SIZE] = "Hello!";
  struct mq_attr attr = {0, MAX_MSG, MSG_SIZE, 0};

  mqd_t fd = mq_open(name, O_CREAT | O_RDWR | O_NONBLOCK,
                     S_IRWXU | S_IRWXG | S_IRWXO, &attr);
  if (fd == (mqd_t)-1) {
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (mq_send(fd, message, strlen(message), 0) == -1) {
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  char buf[MSG_SIZE];
  unsigned priority;
  if (mq_receive(fd, buf, MSG_SIZE, &priority) == -1) {
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("%s\n", buf);

  if (mq_close(fd) == -1) {
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (mq_unlink(name) == -1) {
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}