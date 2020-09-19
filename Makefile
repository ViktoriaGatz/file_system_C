CC := gcc -Wall -Werror
FLAGS := -std=c99
FLAG := -std=c11
SRC_DIR := ./src
BIN_DIR := ./bin
OBJ_DIR := ./obj

.PHONY: all clean install uninstall rebuild

all: $(BIN_DIR)/proc

$(BIN_DIR)/proc: $(OBJ_DIR)/main.o $(OBJ_DIR)/fileio.o
	$(CC) $(OBJ_DIR)/main.o $(OBJ_DIR)/fileio.o -o $(BIN_DIR)/proc

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/fileio.o: $(SRC_DIR)/fileio.c
	$(CC) -c $(SRC_DIR)/fileio.c -o $(OBJ_DIR)/fileio.o

clean:
	find -name "*.o" -exec rm -rf {} +
	rm -rf ./bin/proc

install:
	sudo cp ./bin/proc /usr/bin/proc

uninstall:
	sudo rm -f /usr/bin/proc

rebuild: clean all
