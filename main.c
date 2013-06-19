#include "handlers_list.h"
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

static const char *device = "/dev/spidev0.0";
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t data_ready = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[])
{
	int fd, i, j;
	pthread_t calc_process;

	fd = open(device, O_RDWR);
	sample_struct sample_buffer;

	pthread_create( &calc_process, NULL, calculation_thread, (void*) &sample_buffer);

	for( j = 0; j < 100000; j++)
	{
		pthread_mutex_lock( &mutex1 );
		for ( i=0; i < SAMPLE_SIZE; i++)
		{
			sample_buffer.V[i] = transfer(fd, 0);
			sample_buffer.I[i] = transfer(fd, 1);
			usleep(250);
		}

		pthread_cond_signal( &data_ready);
		pthread_mutex_unlock( &mutex1);
		//printf("j++\n");
	}
	close(fd);

	return 0;

}
