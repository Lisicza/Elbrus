FLAGS ?= -O2
ifeq ($(origin CC), default)
	CC = gcc
endif

3: 3.c
	$(CC) $(FLAGS) $^ -o $@
	
clean:
	rm 3
