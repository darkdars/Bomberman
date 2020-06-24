all: clean SERV CLI

SERV:
	gcc servidor.c -o serv -lpthread -lncurses
CLI:
	gcc cliente.c -o cli -lpthread -lncurses

clean:
	rm -f serv
	rm -f cli
	rm -f sss
