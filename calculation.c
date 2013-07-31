#include "handlers_list.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>


extern pthread_mutex_t mutex1;

void *calculation_thread(void *sample)
{
	sample_struct sample_temp;
	int i;
	float average0 = 0;
	float average1 = 0;

	while(1)
	{
		pthread_mutex_lock( &mutex1 );
		memcpy(&sample_temp, sample, sizeof(sample_struct));
		pthread_mutex_unlock( &mutex1 );

		for( i=0; i < SAMPLE_SIZE; i++)
		{
			average0 += sample_temp.V[i];
			average1 += sample_temp.I[i];
		}
		average0 = average0 / SAMPLE_SIZE;
		average1 = average1 / SAMPLE_SIZE;
		//printf("%f\n", average);
	}
}