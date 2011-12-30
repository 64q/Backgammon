// Includes standards
#include <string.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
// Includes persos
#include "../include/backgammon.h"
#include "../include/engine.h"

void init_engine(engine_state* e_state, char *nameP1, int typeP1, char *nameP2, int typeP2)
{
	e_state->nameP1 = (char*)malloc(50 * sizeof(char) );
	strcpy(e_state->nameP1, nameP1);
	
	e_state->nameP2 = (char*)malloc(50 * sizeof(char) );
	strcpy(e_state->nameP2, nameP2);
	
	e_state->typeP1 = typeP1;
	e_state->typeP2 = typeP2;
	
	e_state->nb_messages = 0;
	
	e_state->run = true;
	
	init_game( &(e_state->g_state) );
	
	srand(time(NULL));
}



void init_game(SGameState * game_state)
{
	game_state->score = 0;
	game_state->scoreP2 = 0;
	game_state->stake = 0;
	game_state->die1 = 0;
	game_state->die2 = 0;
	 
	//on considère que le joueur 1 est le joueur qui joue dans le sens des aiguilles d'une montre (pour l'affichage)
	game_state->zones[EPos_1].player = EPlayer1;
	game_state->zones[EPos_1].nb_checkers = 2;
	
	game_state->zones[EPos_2].nb_checkers = 0;
	game_state->zones[EPos_3].nb_checkers = 0;
	game_state->zones[EPos_4].nb_checkers = 0;
	game_state->zones[EPos_5].nb_checkers = 0;
	
	game_state->zones[EPos_6].player = EPlayer2;
	game_state->zones[EPos_6].nb_checkers = 5;
	
	game_state->zones[EPos_7].nb_checkers = 0;
	
	game_state->zones[EPos_8].player = EPlayer2;
	game_state->zones[EPos_8].nb_checkers = 3;
	
	game_state->zones[EPos_9].nb_checkers = 0;
	game_state->zones[EPos_10].nb_checkers = 0;
	game_state->zones[EPos_11].nb_checkers = 0;
	
	game_state->zones[EPos_12].player = EPlayer1;
	game_state->zones[EPos_12].nb_checkers = 5;
	
	game_state->zones[EPos_13].player = EPlayer2;
	game_state->zones[EPos_13].nb_checkers = 5;
	
	game_state->zones[EPos_14].nb_checkers = 0;
	game_state->zones[EPos_15].nb_checkers = 0;
	game_state->zones[EPos_16].nb_checkers = 0;
	
	game_state->zones[EPos_17].player = EPlayer1;
	game_state->zones[EPos_17].nb_checkers = 3;
	
	game_state->zones[EPos_18].nb_checkers = 0;
	
	game_state->zones[EPos_19].player = EPlayer1;
	game_state->zones[EPos_19].nb_checkers = 5;
	
	game_state->zones[EPos_20].nb_checkers = 0;
	game_state->zones[EPos_21].nb_checkers = 0;
	game_state->zones[EPos_22].nb_checkers = 0;
	game_state->zones[EPos_23].nb_checkers = 0;
	
	game_state->zones[EPos_24].player = EPlayer2;
	game_state->zones[EPos_24].nb_checkers = 2;
	
	game_state->zones[EPos_OutP1].nb_checkers=0;
	game_state->zones[EPos_BarP1].nb_checkers=0;
	
	game_state->zones[EPos_OutP2].nb_checkers=0;
	game_state->zones[EPos_BarP2].nb_checkers=0;
}



void add_message(engine_state* e_state, char* text, int x, int y, int width, int height, ptr_fct_message function)
{
	e_state->tab[e_state->nb_messages].lines = (char**)malloc(sizeof(char*)*5);
	
	char* tmp = (char*)malloc(sizeof(char)*100);
	strcpy(tmp, text);
	
	char * pch = (char*)malloc(sizeof(char)*100);
	
	pch = strtok (tmp,"\n");
	int i = 0;
	while (pch != NULL && i < 5)
	{	
		e_state->tab[e_state->nb_messages].lines[i] = (char*)malloc(sizeof(char)*30);
		strcpy(e_state->tab[e_state->nb_messages].lines[i], pch);
		pch = strtok (NULL, "\n");
		i++;
	}
	
	e_state->tab[e_state->nb_messages].nb_lines = i;
	
	e_state->tab[e_state->nb_messages].position.x = x;
	e_state->tab[e_state->nb_messages].position.y = y;
	e_state->tab[e_state->nb_messages].position.w = width;
	e_state->tab[e_state->nb_messages].position.h = height;
	e_state->tab[e_state->nb_messages].is_clicked = false;
	e_state->function[e_state->nb_messages] = function;
	
	e_state->nb_messages ++;
	
	free(tmp);
	free(pch);
}

void on_click_listener(engine_state* e_state, double ratio)
{
	int x, y;
	SDL_GetMouseState(&x, &y); 
	x /= ratio;
	y /= ratio;
	
	for(int i =0; i < e_state->nb_messages; i++)
	{
		if(e_state->function[i] != NULL)
		{
			if( x > e_state->tab[i].position.x && x < e_state->tab[i].position.x + e_state->tab[i].position.w 
				&& y > e_state->tab[i].position.y && y < e_state->tab[i].position.y + e_state->tab[i].position.h)
			{
				e_state->tab[i].is_clicked = true;
			}else
			{
				e_state->tab[i].is_clicked = false;
			}
		}
	}
	
	
}
void on_unclick_listener(engine_state* e_state, double ratio)
{
	int x, y;
	SDL_GetMouseState(&x, &y); 
	x /= ratio;
	y /= ratio;
	for(int i =0; i < e_state->nb_messages; i++)
	{
		if(e_state->function[i] != NULL && e_state->tab[i].is_clicked)
		{
			e_state->tab[i].is_clicked = false;
			if( x > e_state->tab[i].position.x && x < e_state->tab[i].position.x + e_state->tab[i].position.w 
			&& y > e_state->tab[i].position.y && y < e_state->tab[i].position.y + e_state->tab[i].position.h)
			{
				e_state->function[i](e_state);
			}
		}
	}
}

void start(engine_state* e_state)
{
	erase_messages(e_state);
	add_message(e_state,"lancer\nles dés", 700, 455, 330, 220, first_to_play);
	
}
void erase_messages(engine_state* e_state)
{
	for(int i = 0; i < e_state->nb_messages; i++)
	{
		for(int j = 5; j < 0; j--)
		{
			free(e_state->tab[i].lines[j]);		
		}
		free(e_state->tab[i].lines);		
	}
	e_state->nb_messages = 0;
	
}

void throw_dice(engine_state* e_state)
{
	e_state->g_state.die1 = rand() % 6 + 1;
	e_state->g_state.die2 = rand() % 6 + 1;
}

void shutdown(engine_state* e_state)
{
	e_state->run = false;
}

void first_to_play(engine_state* e_state)
{
	erase_messages(e_state);
	throw_dice(e_state);
	char tmp[50];
	
	if(e_state->g_state.die1 == e_state->g_state.die2)
	{
		add_message(e_state,"    égalité!\nrelancer le dé", 700, 455, 520, 220, first_to_play);
	}else
	{
		if(e_state->g_state.die1 > e_state->g_state.die2)
		{
			sprintf(tmp, "%s\nva commencer\n    à jouer", e_state->nameP1);
			add_message(e_state,tmp, 700, 455, 520, 300, NULL);
		}else
		{
			sprintf(tmp, "%s\nva commencer\n    à jouer", e_state->nameP2);
			add_message(e_state,tmp, 700, 455, 520, 300, NULL);
		}
			
	}
}
















