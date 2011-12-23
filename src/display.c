// Includes standards
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// Includes SDL
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_ttf.h>
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
	TTF_Init();
	
	
	

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
	strcat(path_img_cp, "icone.png");
	SDL_WM_SetIcon(IMG_Load(path_img_cp), NULL);

	
	/* Chargement de la police */
	strcpy(path_img_cp, path_img);
	strcat(path_img_cp, "font.ttf");
    d_manager->font = TTF_OpenFont(path_img_cp, 80);
    
	
	//chargement des images
	load_images(d_manager);
	
	
	
	//initialisation de la fenêtre
	switch_to_window(d_manager);
	
	SDL_WM_SetCaption("BackToGoMan!", NULL);
	
	//création du buffer qui va contenir toute l'interface en 1920*1080 et qui v être redimensionné dans screen
	d_manager->backBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 1920, 1080, 32,0,0,0,0);
	
}


void interface_display(display_manager* d_manager, SGameState* g_state, player_infos* p_infos)
{
	//on blit tout dans le backbuffer en full hd
	
	//background
	SDL_BlitSurface(d_manager->background, NULL, d_manager->backBuffer, &(d_manager->background_position));
	//pions
	checker_display(d_manager, g_state);
	//nom + score
	infos_display(d_manager, g_state, p_infos);
	//dés
	dices_display(d_manager, g_state->die1, 1515, 475);
	dices_display(d_manager, g_state->die2, 1615, 475);
	
	//on adapte à la bonne résolution
	SDL_Surface *tmp;
	tmp = zoomSurface(d_manager->backBuffer, d_manager->ratio, d_manager->ratio, 1);
	
	SDL_BlitSurface(tmp, NULL, d_manager->screen, &(d_manager->background_position));
	
	SDL_FreeSurface(tmp);
}

void checker_display(display_manager* d_manager, SGameState* g_state)
{
	SDL_Rect checker_position;
	
	//affichage de la partie basse droite
	for(int i = 0; i < 6; i++)
	{
		checker_position.x = 1180 - i * 100;
		checker_position.y = 970;
		for(unsigned int j = 0; j < g_state->zones[i].nb_checkers; j++)
		{
			if(g_state->zones[i].player == EPlayer1)
				SDL_BlitSurface(d_manager->white, NULL, d_manager->backBuffer, &(checker_position));
			
			
			if(g_state->zones[i].player == EPlayer2)
				SDL_BlitSurface(d_manager->black, NULL, d_manager->backBuffer, &(checker_position));
			
			checker_position.y -= 100; 
		}
		
		
	}
	
	//affichage de la patie basse gauche
	for(int i = 6; i < 12; i++)
	{
		checker_position.x = 510 - (i-6) * 100;
		checker_position.y = 970;
		for(unsigned int j = 0; j < g_state->zones[i].nb_checkers; j++)
		{
			if(g_state->zones[i].player == EPlayer1)
				SDL_BlitSurface(d_manager->white, NULL, d_manager->backBuffer, &(checker_position));
			
			
			if(g_state->zones[i].player == EPlayer2)
				SDL_BlitSurface(d_manager->black, NULL, d_manager->backBuffer, &(checker_position));
			
			checker_position.y -= 100;
		}
		
	}
	
	//affichage de la partie haute gauche
	for(int i = 12; i < 18; i++)
	{
		checker_position.x = 10 + 100 * (i-12) ;
		checker_position.y = 10;
		for(unsigned int j = 0; j < g_state->zones[i].nb_checkers; j++)
		{
			if(g_state->zones[i].player == EPlayer1)
				SDL_BlitSurface(d_manager->white, NULL, d_manager->backBuffer, &(checker_position));
			
			
			if(g_state->zones[i].player == EPlayer2)
				SDL_BlitSurface(d_manager->black, NULL, d_manager->backBuffer, &(checker_position));
			
			checker_position.y += 100;
		}
		
	}
	
	
	//affichage de la partie haute droite
	for(int i = 18; i < 24; i++)
	{
		checker_position.x = 680 + 100 * (i-18);
		checker_position.y = 10;
		for(unsigned int j = 0; j < g_state->zones[i].nb_checkers; j++)
		{
			if(g_state->zones[i].player == EPlayer1)
				SDL_BlitSurface(d_manager->white, NULL, d_manager->backBuffer, &(checker_position));
			
			
			if(g_state->zones[i].player == EPlayer2)
				SDL_BlitSurface(d_manager->black, NULL, d_manager->backBuffer, &(checker_position));
			
			checker_position.y += 100;
		}
		
	}
	
	//affichage des pions sortis
	checker_position.x = 1290;
	
	for(unsigned int i = 0; i < g_state->zones[EPos_OutP1].nb_checkers; i++)
	{
		checker_position.y = 470 - i * 15;
		SDL_BlitSurface(d_manager->white_out, NULL, d_manager->backBuffer, &(checker_position));
	}
	
	for(unsigned int i = 0; i < g_state->zones[EPos_OutP2].nb_checkers; i++)
	{
		checker_position.y = 1030 - i * 15;
		SDL_BlitSurface(d_manager->black_out, NULL, d_manager->backBuffer, &(checker_position));
	}
	
	//affichage des pions de la barre du joueur blanc (donc les pions noirs)
	checker_position.y = 244;
	for(unsigned int i = 0; i < g_state->zones[EPos_BarP1].nb_checkers; i++)
	{
		checker_position.x = 1455 + i * 100;
		SDL_BlitSurface(d_manager->black, NULL, d_manager->backBuffer, &(checker_position));
	}
	//inverse
	checker_position.y = 959;
	for(unsigned int i = 0; i < g_state->zones[EPos_BarP2].nb_checkers; i++)
	{
		checker_position.x = 1455 + i * 100;
		SDL_BlitSurface(d_manager->white, NULL, d_manager->backBuffer, &(checker_position));
	}
	
}

void infos_display(display_manager* d_manager, SGameState* g_state, player_infos* p_infos)
{
	SDL_Rect position;
	SDL_Surface* texte;
	SDL_Color noir;
	noir.r = 0;
	noir.g = 0;
	noir.b = 0;
	char* score_string = (char*)malloc(sizeof(char)*4);
	
	/* affichage du type de joueur 2 */
	position.x= 1430;
	position.y= 40;
	
	if(p_infos->typeP1 == IA)
	{
		SDL_BlitSurface(d_manager->icone_ia, NULL, d_manager->backBuffer, &(position));
	}else
	{
		SDL_BlitSurface(d_manager->icone_human, NULL, d_manager->backBuffer, &(position));
	}
	
	/* Ecriture du nom et du score du P1 dans la SDL_Surface "texte" en mode Blended (optimal) */
	position.x= 1515+5;
	position.y= 40+5;
    texte = TTF_RenderText_Blended(d_manager->font, p_infos->nameP1, noir );
	SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(position));
	
	position.x= 1515+5;
	position.y= 125+5;
	sprintf(score_string, "%i", g_state->score);
    texte = TTF_RenderText_Blended(d_manager->font, score_string, noir );
	SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(position));
	
	
	
	/* affichage du type de joueur 2 */
	position.x= 1430;
	position.y= 755;
	
	if(p_infos->typeP2 == IA)
	{
		SDL_BlitSurface(d_manager->icone_ia, NULL, d_manager->backBuffer, &(position));
	}else
	{
		SDL_BlitSurface(d_manager->icone_human, NULL, d_manager->backBuffer, &(position));
	}
	
	/* Ecriture du nom et du score du P2 dans la SDL_Surface "texte" en mode Blended (optimal) */
	position.x= 1515+5;
	position.y= 755+5;
    texte = TTF_RenderText_Blended(d_manager->font, p_infos->nameP2, noir );
	SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(position));
	
	position.x= 1515+5;
	position.y= 840+5;
	sprintf(score_string, "%i", g_state->scoreP2);
    texte = TTF_RenderText_Blended(d_manager->font, score_string, noir );
	SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(position));
}

void dices_display(display_manager* d_manager, unsigned int val, int x, int y)
{

	if(val < 7)
	{
		SDL_Rect pos;
		pos.x = x;
		pos.y = y;
		
		//selection de la zone à afficher
		SDL_Rect sel_sprite;
		sel_sprite.x = val * 80 - 80;
		sel_sprite.y = 0;
		sel_sprite.w = 80;
		sel_sprite.h = 80;

		
		SDL_BlitSurface(d_manager->dice, &sel_sprite, d_manager->backBuffer, &(pos));
	}
	

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

		
		d_manager->ratio = (double)d_manager->res_max[0]/(double)d_manager->background->w;

		d_manager->screen = SDL_SetVideoMode(d_manager->res_max[0], d_manager->res_max[1], 32, SDL_FULLSCREEN|SDL_DOUBLEBUF);
		d_manager->display_mode = FULL_SCREEN;
	}
}

void switch_to_window(display_manager* d_manager)
{
	if(d_manager->display_mode != WINDOW)
	{
		
		d_manager->ratio = (double)d_manager->window_mode_width/(double)d_manager->background->w;
		
		
		
		//on passe en plein écran
		d_manager->screen = SDL_SetVideoMode(d_manager->window_mode_width, d_manager->window_mode_width*9/16, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
		d_manager->display_mode = WINDOW;
	}
}


void load_images(display_manager* d_manager)
{
	
	char path_img_cp[100];
	
	//fond
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "background.png");
	d_manager->background = IMG_Load(path_img_cp);
	

	//pion noir
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "black.png");
	d_manager->black = IMG_Load(path_img_cp);
	

	//pion blanc
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "white.png");

	d_manager->white = IMG_Load(path_img_cp);
	
	//pion noir sorti
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "black_out.png");
	d_manager->black_out = IMG_Load(path_img_cp);
	
	//pion blanc sorti
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "white_out.png");
	d_manager->white_out = IMG_Load(path_img_cp);
	
	
	//icone des joueurs ia
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "ia.png");
	d_manager->icone_ia = IMG_Load(path_img_cp);
	
	//icone des joueurs humain
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "human.png");
	d_manager->icone_human = IMG_Load(path_img_cp);
	
	//sprite des dés
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "dice.png");
	d_manager->dice = IMG_Load(path_img_cp);
	
	
}



