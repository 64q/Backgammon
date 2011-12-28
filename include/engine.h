#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>

#include "backgammon.h"


#define IA 1
#define HUMAN 2

typedef void (*ptr_fct_message)(void);

typedef struct player_infos
{
	char *nameP1;
	int typeP1; 
	
	char *nameP2;
	int typeP2;

}player_infos;

typedef struct message
{
	SDL_Surface *lines[10];
	int nb_lines;
	SDL_Rect position;
	bool is_clicked;
	ptr_fct_message function;
	
}message;

typedef struct list_messages
{
	message tab[10];
	int nb_messages;
	
}list_messages;

//initialise la structure player_infos avec le nom de chaque joueur et son type (IA ou HUMAN)
void init_player(player_infos*, char *nameP1, int typeP1, char *nameP2, int typeP2);

//initialise l'état du jeu au départ
void init_game(SGameState*);

void add_message(TTF_Font * font, list_messages*, char* , int x, int y, int width, int height,  ptr_fct_message function);

void on_click_listener(list_messages* list, double ratio);
void on_unclick_listener(list_messages* list, double ratio);

void throw_dice();
void shutdown();

#endif
