#Zmienne
CC=gcc						    #kompilator
CFLAGS=-I. 					   
DEPS = rnd.h 
OBJ = main.c 

%.o: %.c $(DEPS)
		$(CC) -w -c -o $@ $< $(CFLAGS)

carracan: $(OBJ)
	 	$(CC) -w -o $@ $^ $(CFLAGS)