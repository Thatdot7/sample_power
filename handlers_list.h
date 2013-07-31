#define SAMPLE_SIZE 100

typedef struct {
	int V[SAMPLE_SIZE];
	int I[SAMPLE_SIZE];
} sample_struct;


int transfer(int fd, int adc_channel);
void *calculation_thread(void *sample);