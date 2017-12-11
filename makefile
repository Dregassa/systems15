all: client.c control.c key.h
	gcc -o control control.c key.h
	gcc -o client client.c key.h

clean:
	./control -r
	rm client
	rm control
