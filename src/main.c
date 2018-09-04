#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "dining_philosophers.h"

int chopsticks[7] = {0, 0, 0, 0, 0, 0, 0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void set_state_eating(t_philo *phil) {
  phil->s = EATING;
  chopsticks[phil->id] = 1;
  chopsticks[(phil->id + 1) % NUMBER_OF_PHILOSOPHERS] = 1;
  pthread_mutex_unlock(&mutex);
  printf("Philosopher#%d is now eating\n", phil->id + 1);
  sleep(TIME_TO_EAT);
  phil->remaining_rice--;
  if (!phil->remaining_rice) {
    printf("Philosopher#%d finished his bowl of rice\n", phil->id + 1);
  }
  pthread_mutex_lock(&mutex);  
  chopsticks[phil->id] = 0;
  chopsticks[(phil->id + 1) % NUMBER_OF_PHILOSOPHERS] = 0;
  pthread_mutex_unlock(&mutex);
}

void set_state_resting(t_philo *phil) {
  phil->s = RESTING;
  chopsticks[phil->id] = 0;
  chopsticks[(phil->id + 1) % NUMBER_OF_PHILOSOPHERS] = 0;
  printf("Philosopher#%d is now resting\n", phil->id + 1);  
  pthread_mutex_unlock(&mutex);
}

void set_state_thinking(t_philo *phil, const unsigned int chopstick_id) {
  phil->s = THINKING;
  chopsticks[chopstick_id] = 1;
  pthread_mutex_unlock(&mutex);  
  printf("Philosopher#%d is now thinking\n", phil->id + 1);    
  sleep(TIME_TO_THINK);
  pthread_mutex_lock(&mutex);  
  chopsticks[chopstick_id] = 0;
  pthread_mutex_unlock(&mutex);
}

void *dine(void *vargp)
{
  t_philo *phil = ((t_philo*)vargp);

  while (phil->remaining_rice) {
    pthread_mutex_lock(&mutex);
    if (phil->s != EATING && !chopsticks[phil->id] && !chopsticks[(phil->id + 1) % NUMBER_OF_PHILOSOPHERS]) {
      set_state_eating(phil);
    } else if (phil->s != THINKING && !chopsticks[phil->id]) {
      set_state_thinking(phil, phil->id);
    } else if (phil->s != THINKING && phil->s != EATING && !chopsticks[(phil->id + 1) % NUMBER_OF_PHILOSOPHERS]) {
      set_state_thinking(phil, (phil->id + 1) % NUMBER_OF_PHILOSOPHERS);      
    } else if (phil->s == EATING) {
      set_state_resting(phil);
    } else {
      pthread_mutex_unlock(&mutex);  
    }
  }
  return vargp;
}

void init(t_philo* philosophers, const unsigned int size) {
  for (unsigned int i = 0; i < size; i++) {
    enum state s;
    
    if (!chopsticks[i] && !chopsticks[(i + 1) % NUMBER_OF_PHILOSOPHERS]) {
      s = EATING;
      chopsticks[i] = 1;
      chopsticks[(i + 1) % NUMBER_OF_PHILOSOPHERS] = 1;
    } else {
      s = RESTING;
    }
    t_philo p = {i, s, MAX_RICE};
    philosophers[i] = p;

    pthread_create(&(philosophers[i].thread_id), NULL, dine, &philosophers[i]);
  }
}

int main()
{
  t_philo philosophers[NUMBER_OF_PHILOSOPHERS];

  init(philosophers, NUMBER_OF_PHILOSOPHERS);
  for (unsigned int i = 0; i < NUMBER_OF_PHILOSOPHERS; i++) {
    pthread_join((philosophers[i].thread_id), NULL);
    sleep(1);
  }
  exit(EXIT_SUCCESS);
}
