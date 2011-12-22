// Includes standards
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// Includes SDL
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
// Includes persos
#include "../include/backgammon.h"
#include "../include/display.h"

void init_display(display_manager* d_manager ,char* path_img)
{
	//position du fond
	d_manager->background_position.x = 0;
	d_manager->background_position.y = 0;

	//initialiser SDL
	atexit(SDL_Quit);
	SDL_Init(SDL_INIT_VIDEO);

	//on récupère la résolution max de l'écran
	d_manager->res_max[0] = SDL_GetVideoInfo()->current_w;
	d_manager->res_max[1] = SDL_GetVideoInfo()->current_h;

	//choix de la taille du mode fenetre par défaut (en 16/9 donc on définit seulement la largeur)
	d_manager->window_mode_width = 800;

	//on enregistre le chemin des images pour plus tard
	d_manager->path_img = (char*)malloc((strlen(path_img) + 1) * sizeof(char));

	//contiendra la chaîne concaténée
	strcpy(d_manager->path_img, path_img);
	char path_img_cp[100];

	//on charge chaque image du dossier passé en paramètre

	//icone
	strcpy(path_img_cp, path_img);
	strcat(path_img_cp, "sdl_icone.bmp");
	SDL_WM_SetIcon(IMG_Load(path_img_cp), NULL);



	//fond
	strcpy(path_img_cp, path_img);
	strcat(path_img_cp, "background.png");
	d_manager->background = IMG_Load(path_img_cp);

	//pion noir
	strcpy(path_img_cp, path_img);
	strcat(path_img_cp, "noir.png");
	d_manager->black = IMG_Load(path_img_cp);

	//pion blanc
	strcpy(path_img_cp, path_img);
	strcat(path_img_cp, "blanc.png");
	d_manager->white = IMG_Load(path_img_cp);

	//initialisation de la fenêtre

	switch_to_window(d_manager);
	d_manager->display_mode = WINDOW;

	SDL_WM_SetCaption(WIN_NAME, NULL);
}


void interface_display(display_manager* d_manager)
{
	SDL_BlitSurface(d_manager->background, NULL, d_manager->screen, &(d_manager->background_position));

}

void free_surface(display_manager* d_manager)
{
    SDL_FreeSurface(d_manager->background);
    SDL_FreeSurface(d_manager->white);
    SDL_FreeSurface(d_manager->black);
    SDL_FreeSurface(d_manager->screen);
}

void switch_to_full_screen(display_manager* d_manager)
{
	if(d_manager->display_mode != FULL_SCREEN)
	{
		//on recharge les images qui ont été dégradé par le passage en mode fenêtré
		char path_img_cp[100];
		strcpy(path_img_cp, d_manager->path_img);
		strcat(path_img_cp, "background.png");
		d_manager->background = IMG_Load(path_img_cp);

		d_manager->screen = SDL_SetVideoMode(d_manager->res_max[0], d_manager->res_max[1], 32, SDL_FULLSCREEN|SDL_DOUBLEBUF);
		d_manager->display_mode = FULL_SCREEN;
	}
}

void switch_to_window(display_manager* d_manager)
{
	if(d_manager->display_mode != WINDOW)
	{

		//on dézoom les images
		double zoom = (double)d_manager->window_mode_width/(double)d_manager->background->w;
		d_manager->background = zoomSurface(d_manager->background, zoom, zoom, 1);

		//on passe en plein écran
		d_manager->screen = SDL_SetVideoMode(d_manager->window_mode_width, d_manager->window_mode_width*9/16, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
		d_manager->display_mode = WINDOW;
	}
}

