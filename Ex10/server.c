#include "message.h"

int main() {
  int semid;
  if ((semid = semget(SEM_ID, 1, IPC_CREAT | S_IRWXU | S_IRWXG | S_IRWXO)) <
      0) {
    perror("server: create semaphore:");
    exit(EXIT_FAILURE);
  }

  int shmid;
  if ((shmid = shmget(SHM_ID, sizeof(message_t),
                      IPC_CREAT | S_IRWXU | S_IRWXG | S_IRWXO)) < 0) {
    perror("server: create shared segment:");
    exit(EXIT_FAILURE);
  }

  message_t *msg;
  if ((msg = (message_t *)shmat(shmid, 0, 0)) == NULL) {
    perror("server: shared memory attach:");
  }

  semctl(semid, 0, SETVAL, 0);
  msg->type = MSG_TYPE_EMPTY;

  while (1) {
    if (msg->type != MSG_TYPE_EMPTY) {
      if (semctl(semid, 0, GETVAL, 0)) {
        continue;
      }

      semctl(semid, 0, SETVAL, 1);
      if (msg->type == MSG_TYPE_STRING) {
        printf("%s\n", msg->string);
      }
      if (msg->type == MSG_TYPE_FINISH) {
        break;
      }

      msg->type = MSG_TYPE_EMPTY;
      semctl(semid, 0, SETVAL, 0);
    }
  }

  if (semctl(semid, 0, IPC_RMID, (struct semid_ds *)0) < 0) {
    perror("server: delete semaphore:");
    exit(EXIT_FAILURE);
  }
  shmdt(msg);
  if (shmctl(shmid, IPC_RMID, (struct shmid_ds *)0) < 0) {
    perror("server: delete shared segment:");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}