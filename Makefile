# Makefile du projet Backgammon
# Sert à générer l'executable du
# jeu et à nettoyer le répertoire
# du projet.

# Définition des variables usuelles
CC=gcc
CFLAGS=-W -Wall -std=c99 -g
LDFLAGS=-lSDL -lSDL_image -lSDL_ttf -lSDL_gfx
EXEC=backgammon
LIBNAME=ai

# Path
OBJ=obj/
BIN=bin/
INC=include/
SRC=src/
LIB=lib/

all: ${EXEC}

${EXEC}: ${OBJ}backgammon.o ${OBJ}display.o ${OBJ}engine.o ${OBJ}moves.o
	${CC} -o ${BIN}$@ $^ ${LDFLAGS} -ldl -rdynamic

# Règle pour compiler le programme
${OBJ}backgammon.o: ${SRC}backgammon.c
	${CC} -o $@ -c $^ ${CFLAGS}

# Règle pour compiler le moteur de jeu
${OBJ}engine.o: ${SRC}engine.c
	${CC} -o $@ -c $^ ${CFLAGS}

${OBJ}moves.o: ${SRC}moves.c
	${CC} -o $@ -c $^ ${CFLAGS}
# Règle pour compiler l'UI
${OBJ}display.o: ${SRC}display.c
	${CC} -o $@ -c $^ ${CFLAGS}


# Compilation lib ai
libai:
	${CC} -fPIC -o ${LIB}${LIBNAME}.o -c ${LIB}${LIBNAME}.c ${CFLAGS}
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
