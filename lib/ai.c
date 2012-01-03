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
} ai_chained_moves;

// Définition des variables locales
char ai_name[50];
ai_chained_moves ai_moves[AI_MAX_MOVES];
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
void ai_simulate(ai_chained_moves *ch_moves, int position, int *dice, int moves)

/**
 * Evaluer une configuration de jeu
 */
int ai_evaluate_game();

/**
 * Annuler un coup fictif sur le plateau
 */
void ai_cancel(ai_chained_moves *ch_moves);

/**
 * Initialiser les variables
 */
void ai_init_vars();

// Initialise la librairie
void InitLibrary(char name[50])
{
	strcpy(ai_name, name);
}

// Permet d'initialiser l'IA pour un match
void StartMatch(const unsigned int target_score)
{
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
	
	ai_init_vars();
	
	// 1ère étape, vérifier les pions dans la barre
	
	// 2nde étape, vérifier si on doit rentrer les pions
	
	// 3ème étape, jouer normalement
}

void ai_init_vars()
{
	// Init ai_moves struct
	for (int i = 0; i < AI_MAX_MOVES; i++)
	{
		for (int j = 0; j < 4; j++)
		{	
			ai_moves[i].moves[j].src = -1;
			ai_moves[i].moves[j].dest = -1;
		}
		
		ai_moves[i].bonus = 0;
		ai_moves[i].penalty = 0;
	}
	
	ai_curr_index = 0;
}

void ai_play(int *dice, int moves)
{
	int moves = 2;
	int dice[4] = {ai_game_state->die1, ai_game_state->die2, 0, 0};
	
	ai_chained_moves ch_moves;
	
	// En cas de doublé on modifie en conséquence
	if (ai_game_state->die1 == ai_game_state->die2)
	{
		moves = 4;
		dice[2] = ai_game_state->die1;
		dice[3] = ai_game_state->die1;
	}		
	
	for (int j = EPos_24; j <= EPos_1; j--)
	{
		// L'IA est considéré comme J1
		if (ai_game_state->zones[j].player == EPlayer1)
		{
			ai_init_chained_struct(&ch_moves);
			ai_simulate(ch_moves, j, dice, moves);
		}
	}
}

void ai_simulate(ai_chained_moves *ch_moves, int position, int *dice, int moves)
{
	int src = position, dest = position - dice[4 - moves]	
	
	if (moves != 0)
	{
		if (ai_game_state->zones[dest].nb_checkers <= 0 || ai_game_state->zones[dest].player == EPlayer1)
		{
			ch_moves->moves[4 - moves].src = src;
			ch_moves->moves[4 - moves].dest = dest;
			
			// Modification de la struct des zones
			ai_game_state->zones[src].nb_checkers = ai_game_state->zones[src].nb_checkers - 1;
			ai_game_state->zones[dest].nb_checkers = ai_game_state->zones[dest].nb_checkers + 1;
			ai_game_state->zones[dest].player = EPlayer1;
			
			// Rappel de la fonction de simulation sur le jeu modifié
			ai_simulate(ch_moves, 
		}
	}
}

int ai_evaluate_game()
{

}

void ai_cancel(ai_chained_moves *ch_moves)
{

}

void ai_init_chained_struct(ai_chained_moves *ch_moves)
{
	ch_moves->bonus = 0;
	ch_moves->penalty = 0;
	
	for (int i = 0; i < 4; i++)
	{
		ch_moves->moves[i].src = -1;
		ch_moves->moves[i].dest = -1;
	}
}

