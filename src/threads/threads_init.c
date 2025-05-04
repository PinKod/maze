#include <stdlib.h>

#include "./include/thread_t.h"

thread_pool* thread_pool_init(size_t thread_number) {
  thread_pool* ptr_thread_pool = malloc(sizeof(thread_pool));
  if (!ptr_thread_pool) return NULL;
  int flag = SUCCESS;
  thread* prev = NULL;
  for (size_t i = 0; i < thread_number && flag == SUCCESS; i++) {
    thread* ptr_thread_i = malloc(sizeof(thread));
    pthread_t* ptr_pthread_i = malloc(sizeof(pthread_t));
    if (!ptr_thread_i || !ptr_pthread_i) {
      flag = BAD_ALLOCATION;
      if (ptr_thread_i) free(ptr_thread_i);
      if (ptr_pthread_i) free(ptr_pthread_i);
    } else {
      ptr_thread_i->ptr_pthread = ptr_pthread_i;
      ptr_thread_i->prev = prev;
    }
  }
}