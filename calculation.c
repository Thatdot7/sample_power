#include "handlers_list.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

extern pthread_mutex_t mutex1;
extern pthread_cond_t data_ready;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t logging_ready = PTHREAD_COND_INITIALIZER;

void *calculation_thread(void *sample)
{
	logging_struct log;
	sample_struct sample_temp;
	int i;
	int count = 0;
	float real_power;
	float energy;
	float temp;

	struct timeval current_time;
	long int time_stamp;
	
	gettimeofday(&current_time, 0);
	long int prev_time = current_time.tv_sec * 1000000 + current_time.tv_usec;

	pthread_t log_process;
	pthread_create( &log_process, NULL, logging_thread, (void *) &log );

	while(1)
	{
		pthread_mutex_lock( &mutex1 );
		pthread_cond_wait( &data_ready, &mutex1);
		memcpy(&sample_temp, sample, sizeof(sample_struct));
		pthread_mutex_unlock( &mutex1 );

		real_power = 0;

		for( i=0; i < SAMPLE_SIZE; i++)
		{
			real_power += ((double) sample_temp.V[i] * (double) sample_temp.I[i]);
		}

		pthread_mutex_lock( &mutex2 );
		log.real_power[count] = real_power / SAMPLE_SIZE;
		
		gettimeofday(&current_time, NULL);
		time_stamp = current_time.tv_sec * 1000000 + current_time.tv_usec;

		log.energy[count] = real_power * (time_stamp - prev_time) / 1000000;

		if (count >= PROCESSING_SIZE)
		{
			count = 0;
			pthread_cond_signal( &logging_ready);

		} else {
			count++;
		}
		pthread_mutex_unlock( &mutex2 );
		//printf("Ch1 = %d\tCh2 = %d\n", sample_temp.V[0], sample_temp.I[0]);
		//printf("Real Power = %f\tEnergy = %f\n", log.real_power[count-1], log.energy[count-1]);
	}
}

