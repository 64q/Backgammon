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
#include "../include/moves.h"


void set_parametre( int argc, char *argv[], parametre* param)
{

	int nb_player=0;

	for (int i=0; i < argc; i++)
	{
		if (strcmp(argv[i],"-h")==0 && nb_player== 0)/*-h c'est humain*/
		{
			param->name_player_1=(char*)malloc(strlen(argv[i+1])*sizeof(char));
			strcpy( param->name_player_1,argv[i+1]);
			nb_player=nb_player+1;
			param->type_player_1= HUMAN;
			
		}else if (strcmp(argv[i],"-h")==0 && nb_player==1)
		{
			param->name_player_2=(char*)malloc(strlen(argv[i+1])*sizeof(char));
			strcpy( param->name_player_2,argv[i+1]);
			nb_player=nb_player+1;
			param->type_player_2= HUMAN;
			
		}else if (strcmp(argv[i],"-i")==0 && nb_player== 0)
		{
			param->name_player_1=(char*)malloc(strlen(argv[i+1])*sizeof(char));
			strcpy( param->name_player_1,argv[i+1]);
			nb_player=nb_player+1;
			param->type_player_1=IA;
			
		}else if (strcmp(argv[i],"-i")==0 && nb_player== 1)
		{
			param->name_player_2=(char*)malloc(strlen(argv[i+1])*sizeof(char));
			strcpy(param->name_player_2,argv[i+1]);
			nb_player=nb_player+1;
			param->type_player_2= IA;

		}else if (strcmp(argv[i],"-g")==0 )
		{
			param->style=(char*)malloc(strlen(argv[i+1])*sizeof(char));
			strcpy(param->style,argv[i+1]);
		}

	}
}

void init_engine(engine_state* e_state, char *nameP1, int typeP1, char *nameP2, int typeP2)
{
	bool load = true;
	char error[200];
	char tmp[100];
	strcpy(e_state->player_1.name, nameP1);
	
	strcpy(e_state->player_2.name, nameP2);
	
	e_state->player_1.type = typeP1;
	e_state->player_2.type = typeP2;
	
	e_state->player_1.number = EPlayer1;
	e_state->player_2.number = EPlayer2;
	
	
	e_state->nb_messages = 0;
	void *lib;
	if( typeP1 == IA )
	{
		sprintf(tmp, "./lib/%s.so", nameP1);
		if ((lib = dlopen(tmp, RTLD_LAZY)) == NULL)
		{
			sprintf(error, "erreur dans le\n chargement\nde la librairie de \n%s", nameP1);
			add_message(e_state,error , 550, 200, 700, 350, NULL);
			load = false;
		}else
		{
			load_functions_IA(&(e_state->player_1.functions), lib);
		}
	}
	
	if( typeP2 == IA )
	{
		sprintf(tmp, "./lib/%s.so", nameP2);
		if ((lib = dlopen(tmp, RTLD_LAZY)) == NULL)
		{
			sprintf(error, "erreur dans le\n chargement\nde la librairie de \n%s", nameP2);
			add_message(e_state, error, 550, 600, 700, 350, NULL);
			load = false;
		}else
		{
			load_functions_IA( &(e_state->player_2.functions) , lib);
		}
	}
	

	e_state->run = true;
	e_state->score_to_reach = 3;
	e_state->stake_owner = EPlayer1 + EPlayer2; //2 ne correspond ni a EPlayer1 ni EPlayer2
	e_state->src_selected_checker = -1;
	e_state->nb_error_IA = 0;
	
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
	game_state->stake = 1;
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
	
	//rajouter la partie :
	//si le joueur courant est humain, si il clic sur un pion autorisé, on enregistre le pion en cours de transport ainsi que d'où il vient
	if(e_state->nb_messages == 0)
	{
		if( e_state->is_human_playing &&  e_state->src_selected_checker == -1)
		{
			
			int res = get_selected_checker(e_state, x, y);
			if(res != -1 )
			{
				e_state->src_selected_checker = res;
				e_state->g_state.zones[e_state->src_selected_checker].nb_checkers--;
				set_next_possible_moves(e_state, res);
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
	
	//si le joueur courant est humain
	//on regarde si il était en train de transporter un pion 
	//si il le relache on regarde si il tombe dans un endroit autorisé
	//si oui on ajoute met à jour le plateau
	//si tous les dés on été utilisé, on appel la fonction play_turn
	
	if(e_state->nb_messages == 0)
	{
		if( e_state->is_human_playing &&  e_state->src_selected_checker != -1)
		{
			
			int res = get_selected_checker(e_state, x, y);
			if(res != -1 )
			{
				e_state->src_selected_checker = -1;
				e_state->g_state.zones[res].nb_checkers++;
				e_state->g_state.zones[res].player = e_state->current_player->number;
				
			}
		}
	}
	
}

void start_match(engine_state* e_state)
{
	if( e_state->player_1.type == IA )
	{
		e_state->player_1.functions.init_lib(e_state->player_1.name);
		e_state->player_1.functions.start_match(e_state->score_to_reach);
	}
	if( e_state->player_2.type == IA )
	{
		e_state->player_2.functions.init_lib(e_state->player_2.name);
		e_state->player_2.functions.start_match(e_state->score_to_reach);
	}
	start_game(e_state);
	
}
void start_game(engine_state* e_state)
{
	char tmp[100];
	erase_messages(e_state);
	if(e_state->g_state.score < e_state->score_to_reach && e_state->g_state.scoreP2 < e_state->score_to_reach)
	{	
		add_message(e_state,"lancer\nles dés", 700, 455, 330, 220, first_to_play);
	}
	else
	{
		if( e_state->g_state.score >= e_state->score_to_reach)
		{
			sprintf(tmp, "%s\na remporté la partie!", e_state->player_1.name);
			add_message(e_state,tmp, 700, 455, 330, 220, NULL);
		}
		else
		{
			sprintf(tmp, "%s\na remporté la partie!", e_state->player_2.name);
			add_message(e_state,tmp, 700, 455, 330, 220, NULL);
		}
	}
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
	
	e_state->is_first_turn  = true;
	
	if(e_state->g_state.die1 == e_state->g_state.die2)
	{
		add_message(e_state,"égalité!\nrelancer le dé", 700, 455, 520, 220, first_to_play);
	}else
	{
		
		if(e_state->g_state.die1 > e_state->g_state.die2)
		{
			play_turn(e_state, &(e_state->player_1), &(e_state->player_2));
		}
		else
		{	
			play_turn(e_state, &(e_state->player_2), &(e_state->player_1));
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

void play_turn(engine_state* e_state, player* active_player, player* opponent)
{
	e_state->current_player = active_player;
	e_state->pending_player = opponent;
	char tmp[100];
	
	if( active_player->type == IA )
	{
		e_state->is_human_playing = false;
		if(!e_state->is_first_turn)
		{
			throw_dice(e_state);
		}
		
		SGameState g_state_cpy;
		if(active_player->type == EPlayer1)
		{
			copy_reversed_game_state(&g_state_cpy, &(e_state->g_state) );
		}
		else
		{
			copy_game_state(&g_state_cpy, &(e_state->g_state) );
		}
		
		if(!e_state->is_first_turn)
		{
			//débat sur le doublement de la mise
			if( active_player->number == e_state->stake_owner)
			{
				if( active_player->functions.double_stack(&g_state_cpy) )
				{
					if( opponent->type == IA )
					{
						if(active_player->type == EPlayer1)
						{
							copy_reversed_game_state(&g_state_cpy, &(e_state->g_state) );
						}
						else
						{
							copy_game_state(&g_state_cpy, &(e_state->g_state) );
						}
						if( opponent->functions.take_double(&g_state_cpy) )
						{
							double_stack(e_state);
						}else
						{
							
							give_up(e_state);
						}
					}
					else
					{
						sprintf(tmp, "%s\npropose de doubler la mise,\nacceptez vous?", active_player->name);
						add_message(e_state,tmp, 700, 255, 520, 300, NULL);
						add_message(e_state,"OUI", 700, 255, 520, 140, double_stack);
						add_message(e_state,"NON", 850, 255, 520, 140, give_up);
					}
				}
			}
		}
		
		
		SMove moves[4];
		if(active_player->type == EPlayer1)
		{
			copy_reversed_game_state(&g_state_cpy, &(e_state->g_state) );
		}
		else
		{
			copy_game_state(&g_state_cpy, &(e_state->g_state) );
		}
		active_player->functions.make_decision(&g_state_cpy, moves, false);
		
		
		while(!moves_valid(moves) && e_state->nb_error_IA < 3)
		{
			active_player->functions.make_decision(&g_state_cpy, moves, true);
			e_state->nb_error_IA ++;
		}
		
		if(moves_valid(moves))
		{
			copy_moves(e_state->current_moves, moves);
			
			/*temporaire, en attente de quentin*/
			e_state->current_moves[0].src_point = 0;
			e_state->current_moves[0].dest_point = 3;
			
			e_state->current_moves[1].src_point = 0;
			e_state->current_moves[1].dest_point = 3;
			/**************************************/
			/**************************************/
			/**************************************/
			/**************************************/
			/**************************************/
			if(e_state->is_first_turn)
			{
				sprintf(tmp, "%s\nva jouer", active_player->name);
				add_message(e_state,tmp, 700, 455, 520, 300, make_moves);
				e_state->is_first_turn = false;
			}else
			{
				sprintf(tmp, "%s\nva commencer\nà jouer", active_player->name);
				add_message(e_state,tmp, 700, 455, 520, 300, make_moves);
			}
		}
		else
		{
			sprintf(tmp, "%s\ns'est trompé\ntrois fois!", active_player->name);
			add_message(e_state,tmp, 700, 455, 520, 300, give_up);
		}
	}
	else
	{
		if(!e_state->is_first_turn)
		{
			erase_messages(e_state);
			add_message(e_state,"lancer les\ndés", 700, 455, 520, 300, throw_dice_HUMAN);
			e_state->is_first_turn = false;			
		}else
		{
			e_state->is_human_playing = true;
			calc_moves(&(e_state->g_state), &(e_state->current_possible_moves), &(e_state->nb_current_possible_moves), 0, 0);
	
			erase_messages(e_state);
			sprintf(tmp, "%s\njoue en premier", active_player->name);
			add_message(e_state,tmp, 700, 455, 520, 300, erase_messages);
		}
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

void copy_reversed_game_state(SGameState* g_state_cpy, SGameState* g_state)
{
	for(int i = 0; i < 24; i++)
	{
		g_state_cpy->zones[i].player = g_state->zones[23 - i].player;
		g_state_cpy->zones[i].nb_checkers = g_state->zones[23 - i].nb_checkers;
	}
	
	g_state_cpy->zones[EPos_OutP1].player = g_state->zones[EPos_OutP2].player;
	g_state_cpy->zones[EPos_OutP1].nb_checkers =g_state->zones[EPos_OutP2].nb_checkers;
	
	g_state_cpy->zones[EPos_OutP2].player = g_state->zones[EPos_OutP1].player;
	g_state_cpy->zones[EPos_OutP2].nb_checkers =g_state->zones[EPos_OutP1].nb_checkers;
	
	
	g_state_cpy->zones[EPos_BarP1].player = g_state->zones[EPos_BarP2].player;
	g_state_cpy->zones[EPos_BarP1].nb_checkers =g_state->zones[EPos_BarP2].nb_checkers;
	
	g_state_cpy->zones[EPos_BarP2].player = g_state->zones[EPos_BarP1].player;
	g_state_cpy->zones[EPos_BarP2].nb_checkers =g_state->zones[EPos_BarP1].nb_checkers;
	
	g_state_cpy->die1 = g_state->die1;
	g_state_cpy->die2 = g_state->die2;
	
	g_state_cpy->score = g_state->scoreP2;
	g_state_cpy->scoreP2 = g_state->score;
	
	g_state_cpy->stake = g_state->stake;
}

void double_stack(engine_state* e_state)
{
	erase_messages(e_state);
	e_state->g_state.stake *= 2;
	if( e_state->current_player == EPlayer1)
	{
		e_state->stake_owner = EPlayer2;
	}
	else
	{
		e_state->stake_owner = EPlayer1;
	}
}

int moves_valid(SMove moves[4])
{
	return true;
	
}

void copy_moves(SMove cpy[4], SMove original[4])
{
	for( int i = 0; i < 4; i++)
	{
		cpy[i] = original[i];
	}	
}

void make_moves(engine_state* e_state)
{
	/*
	for(int i = 0; i < e_state->nb_current_moves; i++)
	{
		e_state->g_state.zones[e_state->current_moves[i].src_point].nb_checkers --;
		e_state->g_state.zones[e_state->current_moves[i].dest_point].nb_checkers ++;
		e_state->g_state.zones[e_state->current_moves[i].dest_point].player = e_state->current_player->number;
	}*/
	printf("%s a joué\n", e_state->current_player->name);
	erase_messages(e_state);
	if(e_state->g_state.zones[EPos_OutP1].nb_checkers >= 15)
	{
		
	}
	else if(e_state->g_state.zones[EPos_OutP2].nb_checkers >= 15)
	{
		
	}
	else
	{
		play_turn(e_state, e_state->pending_player, e_state->current_player);
	}
}

//c'est le joueur qui n'est pas en train de jouer qui abandonne!!
void give_up(engine_state* e_state)
{
	char tmp[100];
	player* loser;
	if( e_state->player_1.number == e_state->current_player->number )
	{
		loser = &(e_state->player_2);
		e_state->g_state.score += e_state->g_state.stake;
	}
	else
	{
		loser = &(e_state->player_1);
		e_state->g_state.scoreP2 += e_state->g_state.stake;
	}
	
	sprintf(tmp, "%s\nabandonne la manche", loser->name);
	erase_messages(e_state);
	add_message(e_state,tmp, 700, 255, 520, 300, start_game);

}



void current_player_win_game(engine_state* e_state)
{
	player* winner;
	char tmp[100];
	
	if( e_state->player_1.number == e_state->current_player->number )
	{
		winner = &(e_state->player_1);
		e_state->g_state.score += e_state->g_state.stake;
	}
	else
	{
		winner = &(e_state->player_2);
		e_state->g_state.scoreP2 += e_state->g_state.stake;
	}
	sprintf(tmp, "%s\ngagne la manche", winner->name);
	erase_messages(e_state);
	add_message(e_state,tmp, 700, 255, 520, 300, start_game);
}

int get_selected_checker(engine_state* e_state, int x, int y)
{
	for(int i = 0; i < e_state->nb_current_possible_moves; i++)
	{
		
		switch(e_state->current_possible_moves[i].head.src_point)
		{
			case EPos_1:
				if(x <= 1280 && x > 1180 && y >=570 && y < 1070)
					return EPos_1;
				break;
			case EPos_2:
				if(x <= 1180 && x > 1080 && y >=570 && y < 1070)
					return EPos_2;
				break;
			case EPos_3:
				if(x <= 1080 && x > 980 && y >=570 && y < 1070)
					return EPos_3;
				break;
			case EPos_4:
				if(x <= 980 && x > 880 && y >=570 && y < 1070)
					return EPos_4;
				break;
			case EPos_5:
				if(x <= 880 && x > 780 && y >=570 && y < 1070)
					return EPos_5;
				break;
			case EPos_6:
				if(x <= 780 && x > 680 && y >=570 && y < 1070)
					return EPos_6;
				break;
			case EPos_7:
				if(x <= 610 && x > 510 && y >=570 && y < 1070)
					return EPos_7;
				break;
			case EPos_8:
				if(x <= 510 && x > 410 && y >=570 && y < 1070)
					return EPos_8;
				break;
			case EPos_9:
				if(x <= 410 && x > 310 && y >=570 && y < 1070)
					return EPos_9;
				break;
			case EPos_10:
				if(x <= 310 && x > 210 && y >=570 && y < 1070)
					return EPos_10;
				break;
			case EPos_11:
				if(x <= 210 && x > 110 && y >=570 && y < 1070)
					return EPos_11;
				break;
			case EPos_12:
				if(x <= 110 && x > 10 && y >=570 && y < 1070)
					return EPos_12;
				break;
			case EPos_24:
				if(x <= 1280 && x > 1180 && y >=10 && y < 510)
					return EPos_24;
				break;
			case EPos_23:
				if(x <= 1180 && x > 1080 && y >=10 && y < 510)
					return EPos_23;
				break;
			case EPos_22:
				if(x <= 1080 && x > 980 && y >=10 && y < 510)
					return EPos_22;
				break;
			case EPos_21:
				if(x <= 980 && x > 880 && y >=10 && y < 510)
					return EPos_21;
				break;
			case EPos_20:
				if(x <= 880 && x > 780 && y >=10 && y < 510)
					return EPos_20;
				break;
			case EPos_19:
				if(x <= 780 && x > 680 && y >=10 && y < 510)
					return EPos_19;
				break;
			case EPos_18:
				if(x <= 610 && x > 510 && y >=10 && y < 510)
					return EPos_18;
				break;
			case EPos_17:
				if(x <= 510 && x > 410 && y >=10 && y < 510)
					return EPos_17;
				break;
			case EPos_16:
				if(x <= 410 && x > 310 && y >=10 && y < 510)
					return EPos_16;
				break;
			case EPos_15:
				if(x <= 310 && x > 210 && y >=10 && y < 510)
					return EPos_15;
				break;
			case EPos_14:
				if(x <= 210 && x > 110 && y >=10 && y < 510)
					return EPos_14;
				break;
			case EPos_13:
				if(x <= 110 && x > 10 && y >=10 && y < 510)
					return EPos_13;
				break;
			default:
				return -1;
		}
		
	}
	return -1;
}

void set_next_possible_moves(engine_state* e_state, int checker)
{
	
	int i = 0;
	while(i < e_state->nb_current_possible_moves && e_state->current_possible_moves[i].head.dest_point != checker)
	{
		i++;
		printf("salzqazd \n");
	}
	if(i < e_state->nb_current_possible_moves)
	{
		printf("salut \n");
		e_state->current_possible_moves = e_state->current_possible_moves[i].nexts;
		e_state->nb_current_possible_moves = e_state->current_possible_moves[i].l_nexts;
	}
}

void throw_dice_HUMAN(engine_state* e_state)
{
	erase_messages(e_state);
	throw_dice(e_state);
	e_state->is_human_playing = true;
	calc_moves(&(e_state->g_state), &(e_state->current_possible_moves), &(e_state->nb_current_possible_moves), 0, 0);
	//print_poss_moves(&(e_state->current_possible_moves), e_state->nb_current_possible_moves,2);
}






