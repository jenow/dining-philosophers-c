#ifndef _DINING_PHILOSOPHERS_H_
#define _DINING_PHILOSOPHERS_H_

#include <pthread.h>

#define NUMBER_OF_PHILOSOPHERS 7
#define MAX_RICE 5
#define TIME_TO_EAT (rand() % 3)
#define TIME_TO_THINK (rand() % 3)

enum state {RESTING, EATING, THINKING};

typedef struct s_philo {
  unsigned int id;
  enum state s;
  unsigned int remaining_rice;
  pthread_t thread_id;
} t_philo;

extern int chopsticks[7];
extern pthread_mutex_t mutex;

#endif