ksamp : ksamp.o
	gcc $(CFLAGS) -o ksamp ksamp.o

ksamp.o : ksamp.c
	gcc $(CFLAGS) -c ksamp.c

clean :
	rm ksamp ksamp.o
