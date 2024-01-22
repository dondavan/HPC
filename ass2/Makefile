CURR_DIR=$(notdir $(basename $(shell pwd)))
PRJ=$(CURR_DIR)
SRC=$(filter-out $(wildcard ref*.c), $(wildcard *.c))
OBJ=$(patsubst %.c,%.o,$(SRC))

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	CC=clang -Xpreprocessor
else
	CC=gcc
endif

INCLUDES=-I../Include
ifndef DEBUG
CFLAGS=-O2 -std=gnu99 -fopenmp
LIB=
else
CFLAGS=-O0 -g3 -std=gnu99 -fopenmp -fsanitize=thread
LIB=
endif


all: $(PRJ)

$(PRJ): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ $(LIB)	

clean:
	-rm -f $(OBJ) $(PRJ)
