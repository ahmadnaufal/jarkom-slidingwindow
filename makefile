all: transmitter receiver

transmitter: transmitter.c
	gcc -o transmitter transmitter.c -lpthread

receiver: receiver.c
	gcc -o receiver receiver.c -lpthread

clean:
	$(RM) transmitter receiver