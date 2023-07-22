#include "message.h"

int main() {
  int semid;
  if ((semid = semget(SEM_ID, 1, 0)) < 0) {
    perror("client: get semaphores:");
    exit(EXIT_FAILURE);
  }

  int shmid;
  if ((shmid = shmget(SHM_ID, 1, 0)) < 0) {
    perror("client: get memory segment:");
    exit(EXIT_FAILURE);
  }

  message_t *msg;
  if ((msg = (message_t *)shmat(shmid, 0, 0)) == NULL) {
    perror("client: shared memory attach");
    exit(EXIT_FAILURE);
  }

  char str[MAX_STRING_LEN];
  while (1) {
    scanf("%s", str);
    while (semctl(semid, 0, GETVAL, 0) || msg->type != MSG_TYPE_EMPTY)
      ;

    semctl(semid, 0, SETVAL, 1);
    if (strlen(str) != 0) {
      msg->type = MSG_TYPE_STRING;
      strncpy(msg->string, str, MAX_STRING_LEN);
    } else {
      msg->type = MSG_TYPE_FINISH;
    }
    semctl(semid, 0, SETVAL, 0);
    if (strlen(str) == 0) {
      break;
    }
  }

  shmdt(msg);
  exit(EXIT_SUCCESS);
}