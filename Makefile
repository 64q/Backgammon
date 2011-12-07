# Makefile du projet Backgammon
# Sert à générer l'executable du
# jeu et à nettoyer le répertoire
# du projet.

# Définition des variables usuelles
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
