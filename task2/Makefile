FLAGS ?= -O2 -pthread
ifeq ($(origin CC), default)
	CC = gcc
endif

dialog: dialog.c
	$(CC) $(FLAGS) $^ -o $@
	
clean:
	rm -f dialog
