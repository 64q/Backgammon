# Makefile du projet Backgammon
# Sert � g�n�rer l'executable du
# jeu et � nettoyer le r�pertoire
# du projet.

# D�finition des variables usuelles
CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c99
LIB=
EXEC=backgammon

# Path
OBJ=obj/
BIN=bin/
INC=include/
SRC=src/

all:
	@echo "Compilation du projet"
	
clean:
	@echo "Nettoyage du projet"
	rm ${OBJ}*.o

mrproper:
	@echo "Nettoyage de l'executable"
	rm ${BIN}${EXEC}	

# Fin
