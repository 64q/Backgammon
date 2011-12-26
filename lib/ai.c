// Includes standards
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// Includes persos
#include "../include/backgammon.h"
#include "ai.h"

#define AI_MAX_MOVES 50
// Les bonus
#define AI_BON_SECURE 2
#define AI_BON_TAKE 1
#define AI_BON_MOVE 0
// Les pénalités
#define AI_PEN_NO 0
#define AI_PEN_ALONE 1

// Définition des structures locales
typedef struct {
	int src;
	int dest;
	int bonus;
	int penalty;
} ai_move;

// Définition des variables locales
char ai_name[50];
ai_move ai_moves[AI_MAX_MOVES];
SGameState* ai_game_state;
unsigned int ai_target_score;

// Définition des fonctions locales
void ai_update_moves();
int validate_move(const SGameState * const gameState, ai_move move);

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
			ai_moves[i].bonus = 0;
			ai_moves[i].penalty = 0;
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
	return 0;
}

// Retourner une liste de mouvements
void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError)
{
	ai_game_state = gameState;
}

// -----------------
// Fonctions perso
void ai_update_moves()
{
	int curr = 0, is_movable = 0;
	int dice[2] = {ai_game_state->die1, ai_game_state->die2};
	// Parcourt du plateau de jeu
	for (unsigned int i = EPos_24; i <= EPos_1; i--)
	{
		for (unsigned int j = 0; j < 2; j++)
		{
			if (ai_game_state->zones[i].player == EPlayer1 && ai_game_state->zones[i].nb_checkers > 0)
			{
				// Listons à présent les mouvements possible depuis la case i
				// Dé 1
				if (ai_game_state->zones[i - dice[j]].player == EPlayer2 && ai_game_state->zones[i - dice[j]].nb_checkers <= 1)
				{
					ai_moves[curr].src = i;
					ai_moves[curr].dest = i - dice[j];
					ai_moves[curr].bonus = AI_BON_MOVE;
				
					is_movable = 1;
				}
				else if (ai_game_state->zones[i - dice[j]].player == EPlayer1)
				{
					ai_moves[curr].src = i;
					ai_moves[curr].dest = i - dice[j];
				
					is_movable = 1;
				}
			
				// Si deplacement possible, calcul de la pénalité de move
				if (is_movable)
				{
					// Mouvement risqué, ajout d'une pénalité
					if (ai_game_state->zones[i].nb_checkers == 2)
					{
						ai_moves[curr].penalty = AI_PEN_ALONE;
					}
					else if ((ai_game_state->zones[i - dice[j]].nb_checkers + 1) >= 2)
					{
						ai_moves[curr].bonus += AI_BON_SECURE;
					} 
				
					is_movable = 0;
					curr = curr + 1;
				}
			}
		}
	}
}

int validate_move(const SGameState * const gameState, ai_move move)
{

}

