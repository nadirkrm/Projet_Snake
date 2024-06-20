all : run

run : build
	./programme

build : main.o clientAPI.o snakeAPI.o
	gcc -g -o programme main.o clientAPI.o snakeAPI.o

main.o : main.c 
	gcc -g -o main.o -c main.c

clientAPI.o : clientAPI.c snakeAPI.c
	gcc -g -o clientAPI.o -c clientAPI.c

snakeAPI.o : snakeAPI.c
	gcc -g -o snakeAPI.o -c snakeAPI.c 

clean : 
	rm -f *.o
	rm -f programme