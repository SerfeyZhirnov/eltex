#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define IN_PATH "./in"
#define OUT_PATH "./out"

typedef struct {
  char command[BUFFER_SIZE];
  char args[BUFFER_SIZE];
} commands;

int count_commands(char *);
void parse_args(char *, commands *);
void execute_command(commands *, int, int);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: ./command_interpreter \"<command> | <command>\"\n");
    exit(EXIT_FAILURE);
  }
  int count = count_commands(argv[1]);
  commands array[count];
  parse_args(argv[1], array);

  mkfifo(IN_PATH, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  mkfifo(OUT_PATH, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

  for (int i = 0; i < count; ++i) {
    printf("|%s||%s|\n", array[i].command, array[i].args);
    execute_command(array, i, count);
    printf("stage %d ready!\n", i);
  }

  unlink(OUT_PATH);
  unlink(IN_PATH);

  return 0;
}

int count_commands(char *argv) {
  int count = 1;
  for (int i = 0; argv[i] != '\0'; ++i) {
    if (argv[i] == '|') {
      ++count;
    }
  }
  return count;
}

void parse_args(char *argv, commands *array) {
  char *token = strtok(argv, "|");
  int i = 0;
  while (token != NULL) {
    char *pos = strchr(token, ' ');
    if (pos) {
      strncpy(array[i].command, token, strlen(token) - strlen(pos));
      array[i].command[strlen(token) - strlen(pos)] = '\0';
      strcpy(array[i].args, pos + 1);
      if (pos = strchr(array[i].args, ' ')) {
        *pos = '\0';
      }
    } else {
      strcpy(array[i].command, token);
      array[i].args[0] = '\0';
    }

    ++i;
    token = strtok(NULL, "|");
  }
}

void execute_command(commands *array, int index, int count) {
  pid_t pid;
  pid = fork();

  if (pid == 0) {
    int status;
    if (count == 1) {
      if (array[index].args[0] == '\0') {
        status = execlp(array[index].command, array[index].args, NULL);
      } else {
        status = execlp(array[index].command, array[index].command,
                        array[index].args, NULL);
      }
    } else if (index == 0) {
      int fd_out = open(OUT_PATH, O_WRONLY);
      dup2(fd_out, STDOUT_FILENO);
      close(fd_out);

      status = execlp(array[index].command, array[index].command,
                      array[index].args, NULL);
    } else {
      int fd_in = open(IN_PATH, O_RDONLY);
      int fd_out = open(OUT_PATH, O_WRONLY);
      dup2(fd_in, STDIN_FILENO);
      dup2(fd_out, STDOUT_FILENO);
      close(fd_in);
      close(fd_out);

      execlp(array[index].command, array[index].command, array[index].args,
             NULL);
    }

    if (count != 1 && index == count - 1) {
      int fd_in = open(IN_PATH, O_RDONLY);

      char buf[BUFFER_SIZE];
      ssize_t n_read;

      while ((n_read = read(fd_in, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, n_read);
      }

      close(fd_in);
    }

    if (status == -1) {
      printf("%s\n", strerror(errno));
    }

    exit(EXIT_SUCCESS);
  }

  int status;
  wait(&status);
}