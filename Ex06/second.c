#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void PrintProcessStatus();
void WaitProcess();
void CreateProcess();

int main() {
  CreateProcess(CreateProcess, NULL);
  CreateProcess(CreateProcess, CreateProcess);

  WaitProcess();
  WaitProcess();

  PrintProcessStatus();
  return EXIT_SUCCESS;
}

void PrintProcessStatus() {
  printf("PID - %d!\n", getpid());
  printf("Parent PID - %d!\n\n", getppid());
}

void WaitProcess() {
  int status;
  pid_t pid = wait(&status);
  if (pid == -1) {
    exit(EXIT_FAILURE);
  }
  if (WIFEXITED(status)) {
    printf("Child exit-code = %d\n", WEXITSTATUS(status));
  }
}

void CreateProcess(void (*left)(void *, void *),
                   void (*right)(void *, void *)) {
  pid_t pid = fork();
  if (pid == 0) {
    PrintProcessStatus();
    left(NULL, NULL);
    right(NULL, NULL);
    WaitProcess();
    WaitProcess();
    exit(EXIT_SUCCESS);
  }
}