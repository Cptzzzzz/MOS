all: code/main.c code/fibo.c
	gcc -c code/main.c -I include -o code/main.o; \
	gcc -c code/fibo.c -I include -o code/fibo.o; \
 	gcc code/main.o code/fibo.o -o fibo
clean: code/main.o code/fibo.o
	rm code/main.o; \
	rm code/fibo.o
