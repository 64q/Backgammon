#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "backgammon.h"

#define WINDOW 1
#define FULL_SCREEN 2

typedef struct display_manager
{
	//contiendra le contenu de la fenetre
    SDL_Surface *screen;
	
	//les différentes images qui vont être utilisées
    SDL_Surface *background;
    SDL_Surface *black;
    SDL_Surface *white;
	
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

}display_manager;


//initialise la structure d'affichage et ouvre la fenetre
//path_img correspond au chemin d'accèes des imgages
void init_display(display_manager* d_manager ,char* path_img);

//affiche les pions
void checker_display(display_manager*, SGameState*  );

//affiche l'interface
void interface_display(display_manager*);

//libère les SDL_Surface
void free_surface(display_manager*);

//change le mode d'affichage en plein écran
void switch_to_full_screen(display_manager*);

//change le mode d'affichage en mode fenetré
void switch_to_window(display_manager*);

#endif
