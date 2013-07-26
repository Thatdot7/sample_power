#include "handlers_list.h"
#include <stdio.h>
#include <fcntl.h>

static const char *device = "/dev/spidev0.0";

int main(int argc, char *argv[])
{
	int fd, i;

	fd = open(device, O_RDWR);
	setup_chip(fd);

	while(1)
	{
		transfer(fd, 0);
	}

	close(fd);

	return 0;
}
