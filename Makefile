# Makefile du projet Backgammon
# Sert � g�n�rer l'executable du
# jeu et � nettoyer le r�pertoire
# du projet.

# D�finition des variables usuelles
CC=gcc
CFLAGS=-W -Wall -std=c99
LDFLAGS=-lSDL -lSDL_image -lSDL_ttf -lSDL_gfx
EXEC=backgammon
LIBNAME=game_ai

# Path
OBJ=obj/
BIN=bin/
INC=include/
SRC=src/
LIB=lib/

all: ${EXEC}

${EXEC}: ${OBJ}backgammon.o ${OBJ}display.o ${OBJ}engine.o
	${CC} -o ${BIN}$@ $^ ${LDFLAGS} -ldl -rdynamic

# R�gle pour compiler le programme
${OBJ}backgammon.o: ${SRC}backgammon.c
	${CC} -o $@ -c $^ ${CFLAGS}

# R�gle pour compiler le moteur de jeu
${OBJ}engine.o: ${SRC}engine.c
	${CC} -o $@ -c $^ ${CFLAGS}

# R�gle pour compiler l'UI
${OBJ}display.o: ${SRC}display.c
	${CC} -o $@ -c $^ ${CFLAGS}


# Compilation lib ai
libai:
	${CC} -fPIC -o ${LIB}${LIBNAME}.o -c ${LIB}${LIBNAME}.c -I${INC}
	${CC} -shared -o ${LIB}lib${LIBNAME}.so ${LIB}${LIBNAME}.o

# Nettoyage du projet
clean:
	rm ${OBJ}*.o
	rm ${LIB}*.o
	
	
# Nettoyage du projet + rm de l'exe
mrproper: clean
	rm ${LIB}lib${LIBNAME}.so
	rm ${BIN}${EXEC}

# Fin
