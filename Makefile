all: test_sample

test_sample: *o
	gcc *o -o test_sample -lpthread

*o: *c
	gcc -c *c

clean:
	rm -rf *o test_sample
