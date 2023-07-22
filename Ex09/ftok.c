#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MSG_SIZE 256

typedef struct {
  long mtype;
  char mtext[MSG_SIZE];
} msg;

int main() {
  key_t key = ftok(".", 1);
  if (key == -1) {
    printf("%s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  int msgqid = msgget(key, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT);
  if (msgqid == -1) {
    printf("%s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  msg msg_out = {1, "Hello!"};
  if (msgsnd(msgqid, &msg_out, strlen(msg_out.mtext), IPC_NOWAIT) == -1) {
    printf("%s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  msg msg_in;
  if (msgrcv(msgqid, &msg_in, MSG_SIZE, 1, 0) == -1) {
    printf("%s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  printf("%s\n", msg_in.mtext);

  if (msgctl(msgqid, IPC_RMID, NULL)) {
    printf("%s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}