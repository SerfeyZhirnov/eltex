#include <pthread.h>
#include <stdio.h>
#define THREADS 5

void *thread_func(void *arg) {
  int thread_id = *(int *)arg;
  printf("Thread %d is running\n", thread_id);
  pthread_exit((void *)0);
}

int main() {
  int array[THREADS];
  pthread_t thread[THREADS];
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  for (int i = 0; i < THREADS; ++i) {
    array[i] = i;
    pthread_create(&thread[i], &attr, thread_func, (void *)(array + i));
  }

  void *status[THREADS];
  for (int i = 0; i < THREADS; ++i) {
    pthread_join(thread[i], &status[i]);
  }

  for (int i = 0; i < THREADS; ++i) {
    printf("Thread %d exit-status is %d\n", i, *(int *)(status + i));
  }

  return 0;
}