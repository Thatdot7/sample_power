all: test_sample

test_sample: main.o setup.o calculation.o
	gcc *o -o test_sample -lpthread

main.o: main.c
	gcc -c main.c

setup.o: setup.c
	gcc -c setup.c

calculation.o: calculation.c
	gcc -c calculation.c

clean:
	rm -rf *o test_sample
