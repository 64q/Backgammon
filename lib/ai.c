// Includes standards
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// Includes persos
#include "../include/backgammon.h"
#include "ai.h"

#define AI_MAX_MOVES 50

// Définition des structures locales
typedef struct {
	int src;
	int dest;
} ai_move;

// Définition des variables locales
char ai_name[50];
ai_move ai_moves[AI_MAX_MOVES];
SGameState* ai_game_state;
unsigned int ai_target_score;

// Définition des fonctions locales

// Initialise la librairie
void InitLibrary(char name[50])
{
	strcpy(ai_name, name);
}

// Permet d'initialiser l'IA pour un match
void StartMatch(const unsigned int target_score)
{
	// Init ai_moves struct
	for (int i = 0; i < AI_MAX_MOVES; i++)
	{
			ai_moves[i].src = -1;
			ai_moves[i].dest = -1;
	}
	
	// Init vars
	ai_target_score = target_score;
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
	return 1;
}

// Retourner une liste de mouvements
void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError)
{
	ai_game_state = gameState;
}

void ai_play()
{
	int max = -1000;
	int moves = 2;
	int dice[4] = {ai_game_state->dice1, ai_game_state->dice2, 0, 0};
	
	ai_move move;
	
	if (ai_game_state->dice1 == ai_game_state->dice2)
	{
		moves = 4;
		dice[2] = ai_game_state->dice1;
		dice[3] = ai_game_state->dice1;
	}
	
	// Etablissement de tous les coups possibles
	for (int i = 0; i < moves; i++)
	{
		for (int j = EPos_24; j <= EPos_1; j--)
		{
			// L'IA est considéré comme J1
			if (ai_game_state->zones[j].player == EPlayer1)
			{
				if (ai_game_state->zones[j - dice[i]].nb_checkers <= 0)
				{
					ai_simulate(
				}	
			}
		}
	}
}

void ai_simulate()
{

}

void ai_cancel()
{

}

