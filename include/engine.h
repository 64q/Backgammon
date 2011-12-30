#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>

#include "backgammon.h"


#define IA 1
#define HUMAN 2


typedef struct message
{
	char **lines;
	int nb_lines;
	SDL_Rect position;
	bool is_clicked;
	
}message;




typedef struct engine_state
{
	char *nameP1;
	int typeP1; 
	
	char *nameP2;
	int typeP2;
	
	message tab[10];
	void (*function[10])(struct engine_state*);
	int nb_messages;
	
	SGameState g_state;
	
	bool run;

}engine_state;

typedef void (*ptr_fct_message)(engine_state*);




//initialise la structure player_infos avec le nom de chaque joueur et son type (IA ou HUMAN)
void init_engine(engine_state*, char *nameP1, int typeP1, char *nameP2, int typeP2);

//initialise l'état du jeu au départ (private)
void init_game(SGameState*);

void add_message(engine_state* e_state, char* , int x, int y, int width, int height,  ptr_fct_message function);

void on_click_listener(engine_state* e_state, double ratio);
void on_unclick_listener(engine_state* e_state, double ratio);

void start(engine_state* e_state);
void erase_messages(engine_state* e_state);
void throw_dice(engine_state*);
void shutdown(engine_state*);
void first_to_play(engine_state* e_state);
#endif
