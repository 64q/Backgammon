// Includes standards
#include <string.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
// Includes persos
#include "../include/backgammon.h"
#include "../include/engine.h"

void init_player(player_infos* p_infos, char *nameP1, int typeP1, char *nameP2, int typeP2)
{
	p_infos->nameP1 = (char*)malloc(50 * sizeof(char) );
	strcpy(p_infos->nameP1, nameP1);
	
	p_infos->nameP2 = (char*)malloc(50 * sizeof(char) );
	strcpy(p_infos->nameP2, nameP2);
	
	p_infos->typeP1 = typeP1;
	p_infos->typeP2 = typeP2;
	
	
}

void init_game(SGameState * game_state)
{
	game_state->score = 0;
	game_state->scoreP2 = 0;
	game_state->stake = 0;
	 
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



void add_message(TTF_Font * font, list_messages* list, char* text, int x, int y, int width, int height, ptr_fct_message function)
{
	SDL_Color noir;
	noir.r = 0; 
	noir.g = 0;
	noir.b = 0;
	
	char* tmp = (char*)malloc(sizeof(char)*200);
	strcpy(tmp, text);
	
	char * pch = (char*)malloc(sizeof(char)*200);
	
	pch = strtok (tmp,"\n");
	int i = 0;
	while (pch != NULL)
	{	
		
		list->tab[list->nb_messages].lines[i] = TTF_RenderUTF8_Blended(font, pch, noir);

		pch = strtok (NULL, "\n");
		i++;
	}
	
	list->tab[list->nb_messages].nb_lines = i;
	
	list->tab[list->nb_messages].position.x = x;
	list->tab[list->nb_messages].position.y = y;
	list->tab[list->nb_messages].position.w = width;
	list->tab[list->nb_messages].position.h = height;
	list->tab[list->nb_messages].is_clicked = false;
	list->tab[list->nb_messages].function = function;
	
	list->nb_messages ++;
	
}

void on_click_listener(list_messages* list, double ratio)
{
	int x, y;
	SDL_GetMouseState(&x, &y); 
	x /= ratio;
	y /= ratio;
	
	for(int i =0; i < list->nb_messages; i++)
	{
		if( x > list->tab[i].position.x && x < list->tab[i].position.x + list->tab[i].position.w 
			&& y > list->tab[i].position.y && y < list->tab[i].position.y + list->tab[i].position.h)
		{
			list->tab[i].is_clicked = true;
		}else
		{
			list->tab[i].is_clicked = false;
		}
		
	}
	
	
}
void on_unclick_listener(list_messages* list, double ratio)
{
	int x, y;
	SDL_GetMouseState(&x, &y); 
	x /= ratio;
	y /= ratio;
	for(int i =0; i < list->nb_messages; i++)
	{
		if(list->tab[i].is_clicked)
		{
			list->tab[i].is_clicked = false;
			if( x > list->tab[i].position.x && x < list->tab[i].position.x + list->tab[i].position.w 
			&& y > list->tab[i].position.y && y < list->tab[i].position.y + list->tab[i].position.h)
			{
				list->tab[i].function();
			}
		}
	}
}

void throw_dice()
{
	printf("Eh oh minutes!! ça va venir!\n");
}

void shutdown()
{
	exit(0);
}
