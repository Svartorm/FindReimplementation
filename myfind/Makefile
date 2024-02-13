# Makefile

ifeq ($(DEBUG), 1)
	CC = gcc -g -fsanitize=address -DDEBUG
	CFLAGS = -Wall -Wextra -Wvla -std=c99 -pedantic
else
	CC = gcc
	CFLAGS = -Wall -Werror -Wextra -Wvla -std=c99 -pedantic
endif


SRC = $(wildcard src/*.c src/*/*.c)
OBJ = $(SRC:.c=.o)
BIN = myfind

all: $(BIN)

$(BIN): build
	$(CC) -o $(BIN) $(OBJ)

build: $(OBJ)

check:
	cp ./$(BIN) ./tests/
	./tests/test_suite.sh


.PHONY: clean all check
clean:
	${RM} -v src/*.o src/*/*.o
	${RM} -v $(BIN) tests/$(BIN)