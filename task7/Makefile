FLAGS ?= -O2
ifeq ($(origin CC), default)
	CC = gcc
endif

all: 7

7: 7.c
	$(CC) $(FLAGS) $^ -o $@
	
clean:
	rm all