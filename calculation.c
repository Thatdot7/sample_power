#include "handlers_list.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

extern pthread_mutex_t mutex1;
extern pthread_cond_t data_ready;

void *calculation_thread(void *sample)
{
	sample_struct sample_temp;
	int i;
	float average0;
	float average1;
	
	FILE *file;
	file = fopen("data.csv", "a+");

	struct timeval current_time;
	long int time_stamp;

	while(1)
	{
		pthread_mutex_lock( &mutex1 );
		pthread_cond_wait( &data_ready, &mutex1);
		memcpy(&sample_temp, sample, sizeof(sample_struct));
		pthread_mutex_unlock( &mutex1 );

		average0 = 0;
		average1 = 0;

		for( i=0; i < SAMPLE_SIZE; i++)
		{
			average0 += sample_temp.V[i];
			average1 += sample_temp.I[i];
		}
		average0 = average0 / SAMPLE_SIZE;
		average1 = average1 / SAMPLE_SIZE;
		
		gettimeofday(&current_time, NULL);
		time_stamp = current_time.tv_sec * 1000 + current_time.tv_usec;

		fprintf(file, "%d, %f, %f\n", time_stamp, average0, average1);
		//printf("%f\t %f\n", average0, average1);
	}
	fclose(file);
}
