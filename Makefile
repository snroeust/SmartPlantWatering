all: server_web.o
	gcc server_web.o -o server_web

server_web.o: server_web.c
	gcc -c server_web.c

clean:
	rm *.o server_web