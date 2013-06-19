#include "handlers_list.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

extern pthread_mutex_t mutex1;
extern pthread_cond_t data_ready;
extern pthread_mutex_t mutex2;
extern pthread_cond_t logging_ready;

pthread_

void *calculation_thread(void *sample)
{
	logging_struct log;
	sample_struct sample_temp;
	int i;
	int count = 0;
	float real_power;
	float energy;
	float temp;

	FILE *file;
	file = fopen("data.csv", "a+");

	struct timeval current_time;
	long int time_stamp;
	
	gettimeofday(&current_time);
	long int prev_time = current_time.tv_sec * 1000 + current_time.tv_usec;


	while(1)
	{
		pthread_mutex_lock( &mutex1 );
		pthread_cond_wait( &data_ready, &mutex1);
		memcpy(&sample_temp, sample, sizeof(sample_struct));
		pthread_mutex_unlock( &mutex1 );

		real_power = 0;

		for( i=0; i < SAMPLE_SIZE; i++)
		{
			real_power += (sample_temp.V[i] * sample_temp.I[i]);
		}
		log.real_power[count] = real_power / SAMPLE_SIZE;
		
		gettimeofday(&current_time, NULL);
		time_stamp = current_time.tv_sec * 1000 + current_time.tv_usec;

		log.energy[count] = real_power * (time_stamp - prev_time) / 1000;

		if (count >= PROCESSING_SIZE)
		{
			 count = 0;

		} else {
			count++;
		}

		//fprintf(file, "%d, %f, %f\n", time_stamp, average0, average1);
		printf("%f\t %f\n", average0, average1);
	}
	fclose(file);
}
