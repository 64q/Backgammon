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
typedef struct 
{
	int src;
	int dest;
} ai_move;

typedef struct 
{
	ai_move moves[4];
	int bonus;
	int penalty;
} ai_chained_move;

// Définition des variables locales
char ai_name[50];
ai_chained_move ai_moves[AI_MAX_MOVES];
SGameState* ai_game_state;
unsigned int ai_target_score;

// ----------------------------------
// Définition des fonctions locales

/**
 * Faire jouer l'IA de manière virtuelle
 */
void ai_play();

/**
 * Simulation des coups de l'IA
 */
void ai_simulate(ai_move move);

/**
 * Evaluer une configuration de jeu
 */
int ai_evaluate_game();

/**
 * Annuler un coup fictif sur le plateau
 */
void ai_cancel();

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
		for (int j = 0; j < 4; j++)
		{	
			ai_moves[i].moves[j].src = -1;
			ai_moves[i].moves[j].dest = -1;
			ai_moves[i].bonus = 0;
			ai_moves[i].penalty = 0;
		}
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
	
	// 1ère étape, vérifier les pions dans la barre
	
	// 2nde étape, vérifier si on doit rentrer les pions
	
	// 3ème étape, jouer normalement
}

void ai_play(int *dice, int moves)
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
				if (ai_game_state->zones[j - dice[i]].nb_checkers <= 0 || ai_game_state->zones[j - dice[i]].player == EPlayer1)
				{
					move.src = j;
					move.dest = j - dice[i];
					ai_simulate(move);
				}	
			}
		}
	}
}

void ai_simulate(ai_move **moves, int moves)
{
	if (moves == 0)
	{
		
	}
}

int ai_evaluate_game()
{

}

int ai_max(int depth)
{

}

int ai_min(int depth)
{

}

void ai_cancel()
{

}

