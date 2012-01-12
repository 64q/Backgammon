#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>

#include "backgammon.h"
#include "moves.h"


#define IA 1
#define HUMAN 2


typedef struct message
{
	//tableau des lignes composant le message
	char **lines;
	int nb_lines;
	
	//contient la position et la taille du message
	SDL_Rect position;
	
	bool is_clicked;
	
}message;

typedef struct functions
{
	pfInitLibrary init_lib;
	pfStartMatch start_match;
	pfStartGame start_game;
	pfEndGame end_game;
	pfEndMatch end_match;
	pfDoubleStack double_stack;
	pfTakeDouble take_double;
	pfMakeDecision make_decision;
	
}functions;

typedef struct player
{
	char name[50];
	int type; //IA ou HUMAN
	functions functions;
	EPlayer number;
}player;


typedef struct engine_state
{
	player player_1;
	
	player player_2;
	
	//il peut y a voir jusqu'à 10 messages en même temps
	message tab[10];
	//contient la fonction qu iva être lancé quand on clic sur le message correspondant
	void (*function[10])(struct engine_state*);
	int nb_messages;
	
	SGameState g_state;
	
	bool run;
	
	unsigned int score_to_reach;
	
	EPlayer stake_owner;
	
	SMove current_moves[4];
	int nb_current_moves;
	
	player* current_player;
	player* pending_player;
	
	SList_moves *current_possible_moves;
	int nb_current_possible_moves;
	int is_human_playing;
	
	int src_selected_checker;
	
	int nb_error_IA;
	int is_first_turn;

}engine_state;

typedef void (*ptr_fct_message)(engine_state*);

typedef struct parametre
{
	char* name_player_1;
	char*  name_player_2;
	int type_player_1;
	int type_player_2;
	char *style;

}parametre;

void set_parametre( int argc, char *argv[], parametre* param);

//initialise la structure engine_state avec le nom de chaque joueur et son type (IA ou HUMAN)
void init_engine(engine_state* e_state, char *nameP1, int typeP1, char *nameP2, int typeP2);

//initialise l'état du jeu au départ (private)
void init_game(SGameState*);

void add_message(engine_state* e_state, char* , int x, int y, int width, int height,  ptr_fct_message function);

void on_click_listener(engine_state* e_state, double ratio);
void on_unclick_listener(engine_state* e_state, double ratio);

void start_match(engine_state* e_state);
void erase_messages(engine_state* e_state);
void throw_dice(engine_state*);
void shutdown(engine_state*);
void first_to_play(engine_state* e_state);

void load_functions_IA(functions* ptr_functions, void* lib);
void start_game(engine_state* e_state);
void copy_game_state(SGameState* g_state_cpy, SGameState* g_state);
void copy_reversed_game_state(SGameState* g_state_cpy, SGameState* g_state);
void play_turn(engine_state* e_state, player* active_player, player* opponent);

void double_stack(engine_state* e_state);
int moves_valid(SMove moves[4]);
void copy_moves(SMove cpy[4], SMove original[4]);

void make_moves(engine_state* e_state);
void give_up(engine_state* e_state);
void current_player_win_game(engine_state* e_state);
int get_selected_checker(SGameState* g_state);
void throw_dice_HUMAN(engine_state* e_state);
#endif

