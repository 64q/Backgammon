<<<<<<< HEAD
#ifndef ENGINE_H
#define ENGINE_H

#include "backgammon.h"

#define IA 1
#define HUMAN 2

typedef struct player_infos
{
	char *nameP1;
	int typeP1;
	
	char *nameP2;
	int typeP2;

}player_infos;

//initialise la structure player_infos avec le nom de chaque joueur et son type (IA ou HUMAN)
void init_player(player_infos*, char *nameP1, int typeP1, char *nameP2, int typeP2);

//initialise l'état du jeu au départ
void init_game(SGameState*);

#endif
=======
#ifndef __ENGINE_H__
#define __ENGINE_H__



#endif

>>>>>>> 934ef1a350dcd7a01c1a9b8b67c8c988c314b0cd
