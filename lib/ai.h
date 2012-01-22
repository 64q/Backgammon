#ifndef __GAME_AI_H__
#define __GAME_AI_H__

#include "../include/backgammon.h"

/**
 * Initialiser la librairie
 * @param char name[50]
 *	nom associé à la librairie
 */
void InitLibrary(char name[50]);

/**
 * Initialiser l'IA pour un match
 * @param const unsigned int target_score
 *	score cible pour gagner un match
 */
void StartMatch(const unsigned int target_score);

/**
 * Initialiser l'IA pour une manche (d'un match)
 */
void StartGame();

/**
 * Fin d'une manche (d'un match)
 */
void EndGame();

/**
 * Fin d'un match
 */
void EndMatch();

/**
 * Doubler la mise
 * @param const SGameState * const gameState
 *	l'état du jeu courant
 * @return int
 *	vrai si on propose de doubler : faux sinon
 */
int DoubleStack(const SGameState * const gameState);

/**
 * Accepter ou refuser la nouvelle mise
 * @param const SGameState * const gameState
 *	l'état du jeu courant
 * @return int
 *	vrai si on accepte la nouvelle mise ; faux sinon
 */
int TakeDouble(const SGameState * const gameState);

/**
 * Prise de décision de la part de l'IA
 * @param const SGameState * const gameState
 *	l'état du jeu courant
 * @param SMove moves[4]
 *	tableau des mouvements à effectuer par l'IA
 * @param unsigned int lastTimeError
 *	vrai si la dernière action a causée une erreur
 */
void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError);

#endif
