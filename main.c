#include "handlers_list.h"
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <string.h>
#include <stdio.h>

static const char *device = "/dev/spidev0.0";
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
void *calculation_thread(void *sample);

int main(int argc, char *argv[])
{
	int fd, i;
	pthread_t calc_process;

	fd = open(device, O_RDWR);
	sample_struct sample_buffer;

	pthread_create( &calc_process, NULL, calculation_thread, (void*) &sample_buffer);

	while(1)
	{
		pthread_mutex_lock( &mutex1 );
		for ( i=0; i < SAMPLE_SIZE; i++)
		{
			sample_buffer.V[i] = transfer(fd, 0);
			sample_buffer.I[i] = transfer(fd, 1);
			usleep(750);
		}
		pthread_mutex_unlock( &mutex1 );
	}

	close(fd);

	return 0;

}
