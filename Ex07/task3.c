#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define BUYERS_COUNT 3
#define DEFAULT_BUYER_NEED 10000
#define SHOPS_COUNT 5
#define SHOP_DEFAULT_RESOURCE 1000
#define LOAD_VALUE 500
#define BUY_VALUE 500
#define SLEEP_TIME 1

pthread_mutex_t mutexes[SHOPS_COUNT];
int shops[SHOPS_COUNT];
int need_of_buyers[BUYERS_COUNT];
int indexes[BUYERS_COUNT];

void *buy(void *);
int is_need();
void *load(void *);

int main() {
  pthread_mutexattr_t mutex_attr;
  pthread_mutexattr_init(&mutex_attr);
  for (int i = 0; i < SHOPS_COUNT; ++i) {
    shops[i] = SHOP_DEFAULT_RESOURCE;
    pthread_mutex_init(mutexes + i, &mutex_attr);
  }

  pthread_t buyers_threads[BUYERS_COUNT], loader_thread;
  pthread_attr_t thread_attr;
  pthread_attr_init(&thread_attr);

  for (int i = 0; i < BUYERS_COUNT; ++i) {
    need_of_buyers[i] = DEFAULT_BUYER_NEED;
    indexes[i] = i;
    pthread_create(&buyers_threads[i], &thread_attr, buy, (void*)(indexes + i));
  }
  pthread_create(&loader_thread, &thread_attr, load, NULL);

  pthread_join(loader_thread, NULL);
  for (int i = 0; i < BUYERS_COUNT; ++i) {
    pthread_join(buyers_threads[i], NULL);
  }
  
  pthread_attr_destroy(&thread_attr);

  for (int i = 0; i < SHOPS_COUNT; ++i) {
    pthread_mutex_destroy(mutexes + i);
  }
  pthread_mutexattr_destroy(&mutex_attr);
}

void *buy(void *index) {
  int idx = *(int*)index;

  while(need_of_buyers[idx] > 0) {
    for (int i = 0; i < SHOPS_COUNT; ++i) {
      pthread_mutex_lock(&mutexes[i]);
      if (shops[i] >= BUY_VALUE && need_of_buyers[idx] > 0) {
        need_of_buyers[idx] -= BUY_VALUE;
        shops[i] -= BUY_VALUE;
        printf("Buyer %d buy %d in shop %d. Need: %d\n", idx, BUY_VALUE, i, need_of_buyers[idx]);

      }
      pthread_mutex_unlock(&mutexes[i]);
      sleep(SLEEP_TIME);
    }
  }
  pthread_exit(NULL);
}

int is_need() {
  int sum = 0;
  for (int i = 0; i < BUYERS_COUNT; ++i) {
    sum += need_of_buyers[i];
  }
  return sum;
}

void *load(void *args) {
  while (is_need()) {
    for (int i = 0; i < SHOPS_COUNT; ++i) {
      pthread_mutex_lock(&mutexes[i]);
      shops[i] += LOAD_VALUE;
      printf("%d items added to %d shop\n", LOAD_VALUE, i);
      pthread_mutex_unlock(&mutexes[i]);
      sleep(SLEEP_TIME);
    }
  }
  pthread_exit(args);
}