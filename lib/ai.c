// Includes standards
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// Includes persos
#include "../include/backgammon.h"
#include "ai.h"

#define AI_MAX_MOVES 50

// Définition des structures locales
typedef struct {
	int src;
	int dest;
	int bonus;
	int penalty;
} ai_move;

// Définition des variables locales
char ai_name[50];
ai_move moves[AI_MAX_MOVES];

// Initialise la librairie
void InitLibrary(char name[50])
{
	strcpy(ai_name, name);
}

// Permet d'initialiser l'IA pour un match
void StartMatch(const unsigned int target_score)
{
	if (moves == NULL) {
		for (int i = 0; i < AI_MAX_MOVES; i++)
		{
			moves[i].src = -1;
			moves[i].dest = -1;
			moves[i].bonus = 0;
			moves[i].penalty = 0;
		}
	}
}

// Initialiser l'IA pour une partie
void StartGame()
{

}

// Fin d'une partie
void EndGame()
{

}

// Fin d'un match
void EndMatch()
{

}

// Décider si un doublage doit être fait
int DoubleStack(const SGameState * const gameState)
{
	return 0;
}

// Prendre un doublage
int TakeDouble(const SGameState * const gameState)
{
	return 0;
}

// Retourner une liste de mouvements
void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError)
{

}

