#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MESSAGE_SIZE 256

int main() {
  char message[] = "Hello!";
  int fd[2];

  if (pipe(fd) == -1) {
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();

  if (pid == 0) {
    close(fd[0]);
    write(fd[1], message, strlen(message));
    close(fd[1]);
    exit(EXIT_SUCCESS);
  } else {
    close(fd[1]);
    char buffer[MESSAGE_SIZE];
    read(fd[0], buffer, sizeof(buffer));
    printf("%s\n", buffer);
    close(fd[0]);
  }

  return EXIT_SUCCESS;
}