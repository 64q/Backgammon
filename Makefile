# Makefile du projet Backgammon
# Sert � g�n�rer l'executable du
# jeu et � nettoyer le r�pertoire
# du projet.

# D�finition des variables usuelles
CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c99
LIB=-lSDL -lSDL_image -lSDL_ttf
EXEC=backgammon

# Path
OBJ=obj/
BIN=bin/
INC=include/
SRC=src/
LIB=lib/

all:
	@echo "Compilation du projet"
	gcc ${SRC}backgammon.c -o ${BIN}${EXEC} -rdynamic -ldl -I${LIB} -I${INC} ${CFLAGS}

libai:
	@echo "Compilation de l'IA"
	${CC} -fPIC -o ${LIB}game_ai.o -c ${LIB}game_ai.c
	${CC} -shared -o ${LIB}libgame_ai.so.1 ${LIB}game_ai.o -I${INC}

clean:
	@echo "Nettoyage du projet"
	rm ${OBJ}*.o

mrproper:
	@echo "Nettoyage de l'executable"
	rm ${BIN}${EXEC}

# Fin

