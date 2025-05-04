#ifndef THREAD_H_H
#define THREAD_H_H

#include <pthread.h>

#include "./../../includes/defines.h"

struct thread {
  pthread_t* ptr_pthread;
  void* argv;
  void* (*function)(void*);
  void* return_ptr;
  struct thread* next;
  struct thread* prev;
};

typedef struct thread thread;

typedef struct {
  thread* head;
  thread* last;
  size_t thread_number;
} thread_pool;

thread_pool* thread_pool_init(size_t thread_number);

#endif  // THREAD_H_h