#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
 
#include "backgammon.h"
#include "engine.h"

#define WINDOW 1
#define FULL_SCREEN 2
#define WIN_NAME "Backgammon"



typedef struct display_manager
{
	//contiendra le contenu de la fenetre
    SDL_Surface *screen;

	SDL_Surface *backBuffer;
	
	//les différentes images qui vont être utilisées
    SDL_Surface *background;
    SDL_Surface *black;
    SDL_Surface *white;

	SDL_Surface *black_out;
    SDL_Surface *white_out;
	SDL_Surface *icone_ia;
	SDL_Surface *icone_human;
	
	SDL_Surface *dice;
	
	SDL_Surface *stake;
	
	SDL_Surface *message_border;
	SDL_Surface *message_border_clicked;
	
	SDL_Surface* highlight_up;
	SDL_Surface* highlight_down;
	SDL_Surface* highlight_out;
	SDL_Surface* highlight_bar;
	
	int nb_messages_surface;
	SDL_Surface * messages_surface[10];
	SDL_Surface * messages_clicked_surface[10];
	
	//font
	TTF_Font *font;
	
	//la position du fond (0,0)
    SDL_Rect background_position;

	//la résolution max de l'écran (res_max[0]=largeur; res_max[1]=hauteur)
    int res_max[2];

	//le chemin d'accèes des images
    char *path_img;

	//le mode d'affichage (WINDOW ou FULL_SCREEN)
	int display_mode;

	//la largeur du mode fenetré par défaut
	int window_mode_width;
	
	//ratio entre les images (full hd) et la résoltion de la fenêtre
	double ratio;
	
	SDL_Rect moving_checker_pos;
	

} display_manager;




//initialise la structure d'affichage et ouvre la fenetre
//path_img correspond au chemin d'accèes des imgages
void init_display(display_manager* ,char*);


//affiche les pions (private)
void checker_display(display_manager*, SGameState*  );

//affiche les pions
void checker_display(display_manager*, SGameState*);

void moving_checker_display(display_manager* d_manager, engine_state* e_state);

//affiche les scores et autres informations (private)
void infos_display(display_manager*, engine_state*);

//affiche l'interface fond + pion + score ...
void interface_display(display_manager*,engine_state*);

//affiche les dés dans leur zones 
void die_display(display_manager* d_manager, unsigned int val, int x, int y);

void messages_display(display_manager *d_manager, engine_state*);

void stake_display(display_manager *d_manager, engine_state* e_state);

//libère les SDL_Surface
void free_surface(display_manager*);

//change le mode d'affichage en plein écran
void switch_to_full_screen(display_manager*);

//change le mode d'affichage en mode fenetré
void switch_to_window(display_manager*);

//charge toutes les images du jeu (private)
void load_images(display_manager*);

void highlight_possible_moves(display_manager* d_manager, engine_state* e_state);

void messages_display(display_manager *d_manager, engine_state* e_state);
void update_message_surface(display_manager* d_manager, engine_state* e_state);
void create_message_surface(display_manager* d_manager, SDL_Surface* cible, SDL_Surface* bord, char** text, int nb_lines, int width, int height);


#endif

