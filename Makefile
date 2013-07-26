all: test_sample

test_sample: main.o setup.o
	gcc main.o setup.o -o test_sample

main.o: main.c
	gcc -c main.c

setup.o: setup.c
	gcc -c setup.c

clean:
	rm -rf *o test_sample
