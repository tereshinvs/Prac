all: rand.o main.o
	gcc -o genrand main.o rand.o

main.o:
	gcc -c main.c -std=gnu99

rand.o:
	gcc -c rand.c -std=gnu99

clean:
	rm main.o rand.o genrand

