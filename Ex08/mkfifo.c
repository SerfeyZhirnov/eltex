#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define PATH "./mkfifo"
#define MESSAGE_SIZE 256

int main() {
  mkfifo(PATH, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

  pid_t pid = fork();

  if (pid == 0) {
    int fd = open(PATH, O_RDONLY);
    if (fd == -1) {
      exit(EXIT_FAILURE);
    }

    char message[MESSAGE_SIZE];
    int readed = read(fd, message, sizeof(message));
    if (readed == -1) {
      exit(EXIT_FAILURE);
    }
    printf("%s\n", message);

    close(fd);
    exit(EXIT_SUCCESS);
  } else {
    int fd = open(PATH, O_WRONLY);
    if (fd == -1) {
      exit(EXIT_FAILURE);
    }

    char message[] = "Hello!";
    write(fd, message, strlen(message));
    close(fd);
    wait(NULL);
    unlink(PATH);
  }

  return EXIT_SUCCESS;
}