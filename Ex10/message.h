#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SEM_ID 2001
#define SHM_ID 2002

#define MSG_TYPE_EMPTY 0
#define MSG_TYPE_STRING 1
#define MSG_TYPE_FINISH 2

#define MAX_STRING_LEN 256

typedef struct {
  int type;
  char string[MAX_STRING_LEN];
} message_t;