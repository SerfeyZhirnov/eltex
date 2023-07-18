#include <pthread.h>
#include <stdio.h>

#define THREADS 1000
#define VALUE 1000000

unsigned long sum = 0;

void *thread_func(void *arg) {
  for (int i = 0; i < VALUE; ++i) {
    ++sum;
  }
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

  for (int i = 0; i < THREADS; ++i) {
    pthread_join(thread[i], NULL);
  }

  printf("sum = %ld\n", sum);

  return 0;
}