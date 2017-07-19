CC=gcc
BIN_DIR=./bin/
SRC_DIR=./src/

all: makebin processos threads

makebin:
	@mkdir -p $(BIN_DIR)

processos:
	$(CC) -o $(BIN_DIR)processos $(SRC_DIR)processos.c

threads:
	$(CC) -o $(BIN_DIR)threads $(SRC_DIR)threads.c -pthread

clean:
	@rm -rf $(BIN_DIR)