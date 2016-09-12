#
# Makefile ESQUELETO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#  1. Cuidado com a regra "clean" para não apagar o "fila2.o"
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "cthread"
#

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: cthread.o
	ar rcs $(LIB_DIR)/libcthread.a $(BIN_DIR)/cthread.o $(BIN_DIR)/support.o

cthread.o:
	$(CC) -o $(BIN_DIR)/cthread.o $(BIN_DIR)/support.o $(SRC_DIR)/cthread.c $(SRC_DIR)/context.c $(SRC_DIR)/scheduler.c -Wall

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/cthread.o
