CC=gcc
CCFLAGS=-w
ifeq ($(OS),Windows_NT)
    CCFLAGS += -D WINDOWS
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D MACOSX
    endif
endif
	
carracan: main.o
	$(CC) -o $@ $^ $(CCFLAGS)

.PHONY: clean

clean:
	rm -f *.o 
