all: test_sample

test_sample: *o
	gcc *o -o test_sample -lpthread -lsqlite3 -lm

*o: *c
	gcc -c *c

clean:
	rm -rf *o test_sample
