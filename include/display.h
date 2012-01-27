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
	//pions dans la bergerie
	SDL_Surface *black_out;
    SDL_Surface *white_out;
	
	SDL_Surface *icone_ia;
	SDL_Surface *icone_human;
	
	SDL_Surface *dice;
	SDL_Surface *stake;
	
	//bordure et fond des messages en attente
	SDL_Surface *message_border;
	//bordure et fond des messages cliqué
	SDL_Surface *message_border_clicked;
	
	//différents fond qui mettent en valeurs les zones pouvant accueillir ou dont on peut prendre des jetons
	SDL_Surface* highlight_up;
	SDL_Surface* highlight_down;
	SDL_Surface* highlight_out;
	SDL_Surface* highlight_bar;
	
	//indique le joueur en cours
	SDL_Surface* highlight_player;
	
	//nombre de messages chargés
	int nb_messages_surface;
	//les surfacesdes messages comprenant la bordure, le fond et le texte
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
	
	//l'emplacement du pion qui est bougé
	SDL_Rect moving_checker_pos;
	

} display_manager;




//initialise la structure d'affichage et ouvre la fenetre
//path_img correspond au chemin d'accèes des imgages
void init_display(display_manager* ,char*, engine_state*);


//affiche les pions contenu dans le SGameState*
void checker_display(display_manager*, SGameState*);

//affiche le pion qui est en train d'être déplacé par un joueur humain
void moving_checker_display(display_manager* d_manager, engine_state* e_state);

//affiche les scores et autres informations (private)
void infos_display(display_manager*, engine_state*);

//affiche l'interface fond + pion + score ...
void interface_display(display_manager*,engine_state*);

//affiche les dés dans leur zones 
void die_display(display_manager* d_manager, unsigned int val, int x, int y);

//affiche les messages prélablement chargés avec void create_message_surface
void messages_display(display_manager *d_manager, engine_state*);

//affiche la mise courante
void stake_display(display_manager *d_manager, engine_state* e_state);

//libère les SDL_Surface
void free_surface(display_manager*);

//change le mode d'affichage en plein écran
void switch_to_full_screen(display_manager*, engine_state*);

//change le mode d'affichage en mode fenetré
void switch_to_window(display_manager*, engine_state*);

//charge toutes les images du jeu (private)
void load_images(display_manager*);

//met en valeur les zones dont on peut prendre des jetons ou bien qui peuvent en recevoir
void highlight_possible_moves(display_manager* d_manager, engine_state* e_state);

//met à jour les tableaux de surfaces des messages devant être affichés
void update_message_surface(display_manager* d_manager, engine_state* e_state);

//crée la surface d'un message avec les paramètres
//@cible reçoit la surface finale contenant le message
//@bord contient l'image (au format défini dans le style)  qu iva servir de bord et de fond au message
//@text contient le texte du message sous forme de tableau, chaque cellule du tablea représent une ligne
//@nb_lines correspond en fait à la taille de text** et donc au nombre de lignes du message
//@widt largeur du message
//@height hauteur du message
void create_message_surface(display_manager* d_manager, SDL_Surface* cible, SDL_Surface* bord, char** text, int nb_lines, int width, int height);

//intialisation des tableau des messages
void init_messages(display_manager* d_manager);

#endif

