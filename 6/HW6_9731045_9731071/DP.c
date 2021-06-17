#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void *handle_philo(void * i);
void think(int phil);
void eat(int phil);
void finish(int phil);
void pickup(int phil);
void putdown(int phil);

sem_t pick_up;
sem_t put_down;
pthread_t phil[5];
pthread_mutex_t chop[5];
int count[5];

int main(){

	sem_init(&pick_up, 0 , 1);
	sem_init(&put_down, 0, 1);

	for (int i = 0; i < 5; ++i)
	{
		if (pthread_mutex_init(&chop[i], NULL))
		{
			printf("Mutex Initialize Failed\n");
			return 0;
		}
	}
	for (int i = 0; i < 5; ++i)
	{
		pthread_create(&phil[i], NULL, handle_philo, (void *)i);
	}
	for (int i = 0; i < 5; ++i)
	{
		pthread_join(phil[i], NULL);
	}
}

void *handle_philo (void *i){

	int id = (int)(long) i;

	count[id] = 1;
	while(1){
		
		//think
		think(id);
		sleep(2);
		
		//pickup
		sem_wait(&pick_up);
			pickup(id);
			//eat
			eat(id);
		sem_post(&pick_up);

		sleep(2);

		sem_wait(&put_down);
			//finish eating
			finish(id);
			//putdown
			putdown(id);
		sem_post(&put_down);

		count[id]++;
	}
}

void pickup(int phil){
	pthread_mutex_lock(&chop[phil]);
	pthread_mutex_lock(&chop[(phil+1)%5]);
}

void putdown(int phil){
	pthread_mutex_unlock(&chop[phil]);
	pthread_mutex_unlock(&chop[(phil+1)%5]);
}

void think(int phil){
	printf("Phil %d is thinking\n", phil);
}

void eat(int phil){
	printf("Phil %d is eating with chop[%d] and chop[%d] for the [%d]th time\n", phil, phil, (phil+1)%5, count[phil]);

}

void finish(int phil){
	printf("Phil %d finished eating\n", phil);
}
