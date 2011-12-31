// Includes standards
#include <string.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
#include <dlfcn.h>

// Includes persos
#include "../include/backgammon.h"
#include "../include/engine.h"

void init_engine(engine_state* e_state, char *nameP1, int typeP1, char* path_lib_P1, char *nameP2, int typeP2, char* path_lib_P2)
{
	bool load = true;
	char error[200];
	e_state->nameP1 = (char*)malloc(50 * sizeof(char) );
	strcpy(e_state->nameP1, nameP1);
	
	e_state->nameP2 = (char*)malloc(50 * sizeof(char) );
	strcpy(e_state->nameP2, nameP2);
	
	e_state->typeP1 = typeP1;
	e_state->typeP2 = typeP2;
	
	e_state->nb_messages = 0;
	
	if( typeP1 == IA )
	{
		void *lib;
		
		if ((lib = dlopen(path_lib_P1, RTLD_LAZY)) == NULL)
		{
			sprintf(error, "erreur dans le\n chargement\nde la librairie de \n%s", nameP1);
			add_message(e_state,error , 550, 200, 700, 350, NULL);
			load = false;
		}else
		{
			load_functions_IA(&(e_state->functions_P1), lib);
		}
	}else
	{
		
	}
	
	if( typeP2 == IA )
	{
		void *lib;
		
		if ((lib = dlopen(path_lib_P2, RTLD_LAZY)) == NULL)
		{
			sprintf(error, "erreur dans le\n chargement\nde la librairie de \n%s", nameP2);
			add_message(e_state, error, 550, 600, 700, 350, NULL);
			load = false;
		}else
		{
			load_functions_IA( &(e_state->functions_P2) , lib);
		}
	}else
	{
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	e_state->run = true;
	e_state->score_to_reach = 3;
	
	init_game( &(e_state->g_state) );
	
	srand(time(NULL));
	if(load)
	{
		add_message(e_state, "Jouer!" , 760, 455, 400, 170,  start_match );
		add_message(e_state,"Quitter" , 760, 650, 400, 170,  shutdown );
	}else
	{
		add_message(e_state, "Quitter!" , 1450, 455, 400, 170,  shutdown );
	}
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

void start_match(engine_state* e_state)
{
	if( e_state->typeP1 == IA )
	{
		e_state->functions_P1.init_lib(e_state->nameP1);
		e_state->functions_P1.start_match(e_state->score_to_reach);
	}
	if( e_state->typeP2 == IA )
	{
		e_state->functions_P2.init_lib(e_state->nameP1);
		e_state->functions_P2.start_match(e_state->score_to_reach);
	}
	start_game(e_state);
	
}
void erase_messages(engine_state* e_state)
{
	
	for(int i = 0; i < e_state->nb_messages; i++)
	{
		if(e_state->tab[i].lines != NULL)
		{			
			for(int j = 0; j > 5; j++)
			{
				free(e_state->tab[i].lines[j]);		
			}
			free(e_state->tab[i].lines);		
		}
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
		add_message(e_state,"égalité!\nrelancer le dé", 700, 455, 520, 220, first_to_play);
	}else
	{
		if(e_state->g_state.die1 > e_state->g_state.die2)
		{
			sprintf(tmp, "%s\nva commencer\nà jouer", e_state->nameP1);
			add_message(e_state,tmp, 700, 455, 520, 300, play_P1);
		}else
		{
			sprintf(tmp, "%s\nva commencer\nà jouer", e_state->nameP2);
			add_message(e_state,tmp, 700, 455, 520, 300, NULL);
		}
			
	}
}

void load_functions_IA(functions* ptr_functions, void* lib)
{
	ptr_functions->init_lib = dlsym (lib, "InitLibrary");
	ptr_functions->start_match = dlsym (lib, "StartMatch");
	ptr_functions->start_game = dlsym (lib, "StartGame");
	ptr_functions->end_game = dlsym (lib, "EndGame");
	ptr_functions->end_match = dlsym (lib, "EndMatch");
	ptr_functions->double_stack = dlsym (lib, "DoubleStack");
	ptr_functions->take_double = dlsym (lib, "TakeDouble");
	ptr_functions->make_decision = dlsym (lib, "MakeDecision");
	char * error;
	if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "ERREUR LORS DU CHARGEMENT DES FONCTIONS : %s\n", error);
        exit(1);
    }
}

void start_game(engine_state* e_state)
{
	erase_messages(e_state);
	add_message(e_state,"lancer\nles dés", 700, 455, 330, 220, first_to_play);
}

void play_P1(engine_state* e_state)
{
	if( e_state->typeP1 == IA )
	{
		throw_dice(e_state);
		SGameState g_state_cpy;
		copy_game_state(&g_state_cpy, &(e_state->g_state) );
	}
}

void copy_game_state(SGameState* g_state_cpy, SGameState* g_state)
{
	
	for(int i = 0; i < 28; i++)
	{
		g_state_cpy->zones[i].player = g_state->zones[i].player;
		g_state_cpy->zones[i].nb_checkers = g_state->zones[i].nb_checkers;
	}
	g_state_cpy->die1 = g_state->die1;
	g_state_cpy->die2 = g_state->die2;
	
	g_state_cpy->score = g_state->score;
	g_state_cpy->scoreP2 = g_state->scoreP2;
	
	g_state_cpy->stake = g_state->stake;
}
















