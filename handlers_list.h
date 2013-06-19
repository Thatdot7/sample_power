#define SAMPLE_SIZE 3

typedef struct {
	unsigned int V[SAMPLE_SIZE];
	unsigned int I[SAMPLE_SIZE];
} sample_struct;


unsigned int transfer(int fd, int adc_channel);
void *calculation_thread(void *sample);
