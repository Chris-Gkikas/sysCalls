#include <stdio.h>
#include <pthread.h>
#include <unistd.h>	
#define NUMBER_OF_THREADS 10
static long cur_value=0;
static void* increase_value(void* arg){
	cur_value+=6;
}
int main(){
	pthread_t threads[NUMBER_OF_THREADS];
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
	return 0;
}
