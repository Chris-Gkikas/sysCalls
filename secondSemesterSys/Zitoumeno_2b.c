#include <stdio.h>
#include <pthread.h>
#include <unistd.h>	
#define NUMBER_OF_THREADS 10
static long cur_value=0;
pthread_mutex_t mid;
static void* increase_value(void* arg){ 
	if (pthread_mutex_lock(&mid)!=0)
		printf("Error in mutex lock!\n");
	cur_value+=6; 
	if (pthread_mutex_unlock(&mid)!=0)
		printf("Error in mutex unlock!\n");
}
int main(){
	pthread_t threads[NUMBER_OF_THREADS];
	if (pthread_mutex_init(&mid,NULL)!=0){								//mutex initialazation
		printf("Error in mutex initialization!\n");
		return -1;
	}
	for (int i = 0; i < NUMBER_OF_THREADS; i++){
		if (pthread_create(&threads[i],NULL,increase_value,NULL)!=0){	//thread creation
			printf("Error creating thread #%d!\n",i+1);
			return -1;
		}
		printf("thead[%d]	cur_value[%ld]\n",i+1,cur_value);
	}
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_join(threads[i],NULL);									//wait all threads to terminate
	printf("\ncur_value: %ld\n\n",cur_value);
	pthread_mutex_destroy(&mid);
	return 0;
}