#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();
  if (pid == 0) {
    printf("This is child!\n");
    printf("Child PID - %d!\n", getpid());
    printf("Child parent - %d!\n", getppid());
    exit(EXIT_SUCCESS);
  } else {
    printf("This is parent!\n");
    printf("Parent PID - %d!\n", getpid());
    printf("Parent parent - %d!\n", getppid());
  }

  int status;
  pid_t child_pid = wait(&status);

  if (child_pid == -1) {
    exit(EXIT_FAILURE);
  }

  if (WIFEXITED(status)) {
    printf("Child exit-code = %d\n", WEXITSTATUS(status));
  }

  return EXIT_SUCCESS;
}