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
#include "../include/engine.h"
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
    d_manager->font = TTF_OpenFont(path_img_cp, 70);
    
	
	//chargement des images
	load_images(d_manager);

	//initialisation de la fenêtre
	switch_to_window(d_manager);
	
	SDL_WM_SetCaption("BackToGoMan!", NULL);
	
	//création du buffer qui va contenir toute l'interface en 1920*1080 et qui v être redimensionné dans screen
	d_manager->backBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 1920, 1080, 32,0,0,0,0);
	
	
	
}


void interface_display(display_manager* d_manager, engine_state* e_state)
{
	//on blit tout dans le backbuffer en full hd
	
	//background
	SDL_BlitSurface(d_manager->background, NULL, d_manager->backBuffer, &(d_manager->background_position));
	//pions
	checker_display(d_manager, &(e_state->g_state));
	//nom + score
	infos_display(d_manager, e_state);
	//dés
	if(e_state->g_state.die1 > 0 && e_state->g_state.die1 < 7 && e_state->g_state.die2 > 0 && e_state->g_state.die2 < 7)
	{
		die_display(d_manager, e_state->g_state.die1, 1570, 500);
		die_display(d_manager, e_state->g_state.die2, 1660, 500);
		if(e_state->g_state.die1 == e_state->g_state.die2)
		{
			die_display(d_manager, e_state->g_state.die2, 1485, 500);
			die_display(d_manager, e_state->g_state.die2, 1745, 500);
		}
	}
	
	//messages
	messages_display(d_manager, e_state);
	
	//on adapte à la bonne résolution
	SDL_Surface *tmp;
	tmp = zoomSurface(d_manager->backBuffer, d_manager->ratio, d_manager->ratio, 1);
	
	SDL_BlitSurface(tmp, NULL, d_manager->screen, &(d_manager->background_position));
	
	SDL_FreeSurface(tmp);
}

void checker_display(display_manager* d_manager, SGameState* g_state)
{
	SDL_Rect checker_position;
	SDL_Surface* texte;
	char checker_number[3];
	SDL_Color noir;
	noir.r = 0;
	noir.g = 0;
	noir.b = 0;
	
	//affichage de la partie basse droite
	for(int i = 0; i < 6; i++)
	{
		checker_position.x = 1180 - i * 100;
		checker_position.y = 970;
		for(unsigned int j = 0; j < g_state->zones[i].nb_checkers && j < 5; j++)
		{
			if(g_state->zones[i].player == EPlayer1)
				SDL_BlitSurface(d_manager->white, NULL, d_manager->backBuffer, &(checker_position));
			
			
			if(g_state->zones[i].player == EPlayer2)
				SDL_BlitSurface(d_manager->black, NULL, d_manager->backBuffer, &(checker_position));
			
			checker_position.y -= 100; 
		}
		if(g_state->zones[i].nb_checkers > 5)
		{
			checker_position.x += 22;
			checker_position.y += 113;
			sprintf(checker_number, "%i", g_state->zones[i].nb_checkers);
			texte = TTF_RenderText_Blended(d_manager->font, checker_number, noir );
			SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(checker_position));
		}
		
		
	}
	
	//affichage de la patie basse gauche
	for(int i = 6; i < 12; i++)
	{
		checker_position.x = 510 - (i-6) * 100;
		checker_position.y = 970;
		for(unsigned int j = 0; j < g_state->zones[i].nb_checkers && j < 5; j++)
		{
			if(g_state->zones[i].player == EPlayer1)
				SDL_BlitSurface(d_manager->white, NULL, d_manager->backBuffer, &(checker_position));
			
			
			if(g_state->zones[i].player == EPlayer2)
				SDL_BlitSurface(d_manager->black, NULL, d_manager->backBuffer, &(checker_position));
			
			checker_position.y -= 100;
		}
		if(g_state->zones[i].nb_checkers > 5)
		{
			checker_position.x += 22;
			checker_position.y += 113;
			sprintf(checker_number, "%i", g_state->zones[i].nb_checkers);
			texte = TTF_RenderText_Blended(d_manager->font, checker_number, noir );
			SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(checker_position));
		}
	}
	
	
	//affichage de la partie haute gauche
	for(int i = 12; i < 18; i++)
	{
		checker_position.x = 10 + 100 * (i-12) ;
		checker_position.y = 10;
		for(unsigned int j = 0; j < g_state->zones[i].nb_checkers && j < 5; j++)
		{
			if(g_state->zones[i].player == EPlayer1)
				SDL_BlitSurface(d_manager->white, NULL, d_manager->backBuffer, &(checker_position));
			
			
			if(g_state->zones[i].player == EPlayer2)
				SDL_BlitSurface(d_manager->black, NULL, d_manager->backBuffer, &(checker_position));
			
			checker_position.y += 100;
		}
		if(g_state->zones[i].nb_checkers > 5)
		{
			checker_position.x += 22;
			checker_position.y -= 90;
			sprintf(checker_number, "%i", g_state->zones[i].nb_checkers);
			texte = TTF_RenderText_Blended(d_manager->font, checker_number, noir );
			SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(checker_position));
		}
		
	}
	
	//affichage de la partie haute droite
	for(int i = 18; i < 24; i++)
	{
		checker_position.x = 680 + 100 * (i-18);
		checker_position.y = 10;
		for(unsigned int j = 0; j < g_state->zones[i].nb_checkers && j < 5; j++)
		{
			if(g_state->zones[i].player == EPlayer1)
				SDL_BlitSurface(d_manager->white, NULL, d_manager->backBuffer, &(checker_position));
			
			
			if(g_state->zones[i].player == EPlayer2)
				SDL_BlitSurface(d_manager->black, NULL, d_manager->backBuffer, &(checker_position));
			
			checker_position.y += 100;
		}
		if(g_state->zones[i].nb_checkers > 5)
		{
			checker_position.x += 22;
			checker_position.y -= 90;
			sprintf(checker_number, "%i", g_state->zones[i].nb_checkers);
			texte = TTF_RenderText_Blended(d_manager->font, checker_number, noir );
			SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(checker_position));
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

void infos_display(display_manager* d_manager, engine_state* e_state)
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
	
	if(e_state->player_1.type == IA)
	{
		SDL_BlitSurface(d_manager->icone_ia, NULL, d_manager->backBuffer, &(position));
	}else
	{
		SDL_BlitSurface(d_manager->icone_human, NULL, d_manager->backBuffer, &(position));
	}
	
	/* Ecriture du nom et du score du P1 dans la SDL_Surface "texte" en mode Blended (optimal) */
	position.x= 1515+5;
	position.y= 40;
    texte = TTF_RenderUTF8_Blended(d_manager->font, e_state->player_1.name, noir );
	SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(position));
	
	position.x= 1515+20;
	position.y= 125;
	sprintf(score_string, "%i", e_state->g_state.score);
    texte = TTF_RenderText_Blended(d_manager->font, score_string, noir );
	SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(position));
	
	
	
	/* affichage du type de joueur 2 */
	position.x= 1430;
	position.y= 755;
	
	if(e_state->player_2.type == IA)
	{
		SDL_BlitSurface(d_manager->icone_ia, NULL, d_manager->backBuffer, &(position));
	}else
	{
		SDL_BlitSurface(d_manager->icone_human, NULL, d_manager->backBuffer, &(position));
	}
	
	/* Ecriture du nom et du score du P2 dans la SDL_Surface "texte" en mode Blended (optimal) */
	position.x= 1515+5;
	position.y= 755;
    texte = TTF_RenderUTF8_Blended(d_manager->font, e_state->player_2.name, noir );
	SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(position));
	
	position.x= 1515+20;
	position.y= 840;
	sprintf(score_string, "%i", e_state->g_state.scoreP2);
    texte = TTF_RenderText_Blended(d_manager->font, score_string, noir );
	SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(position));
	
	free(score_string);
}

void die_display(display_manager* d_manager, unsigned int val, int x, int y)
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

void messages_display(display_manager *d_manager, engine_state* e_state)
{
	SDL_Surface *border;
	SDL_Surface *text;
	
	SDL_Rect sel_sprite;
	SDL_Rect position;
	
	SDL_Color noir;
	noir.r = 0;
	noir.g = 0;
	noir.b = 0;
	
	for(int i = 0; i < e_state->nb_messages; i++)
	{
		if(e_state->tab[i].is_clicked)
		{
			border = d_manager->message_border_clicked;
			
		}else
		{
			border = d_manager->message_border;
			
		}
		
		//affichage du coin haut gauche
		sel_sprite.x = 0;
		sel_sprite.y = 0;
		sel_sprite.w = 30;
		sel_sprite.h = 30;
		position.x = e_state->tab[i].position.x;
		position.y = e_state->tab[i].position.y;
		
		SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
		
		//affichage du coin haut droit
		sel_sprite.x = 72;
		sel_sprite.y = 0;
		sel_sprite.w = 30;
		sel_sprite.h = 30;
		position.x = e_state->tab[i].position.x + e_state->tab[i].position.w - 30;
		SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
		
		//affichage du coin bas gauche
		sel_sprite.x = 0;
		sel_sprite.y = 72;
		sel_sprite.w = 30;
		sel_sprite.h = 30;
		position.x = e_state->tab[i].position.x;
		position.y = e_state->tab[i].position.y + e_state->tab[i].position.h - 30;
		SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
		
		//affichage du coin bas droit
		sel_sprite.x = 72;
		sel_sprite.y = 72;
		sel_sprite.w = 30;
		sel_sprite.h = 30;
		position.x = e_state->tab[i].position.x + e_state->tab[i].position.w - 30;
		position.y = e_state->tab[i].position.y + e_state->tab[i].position.h - 30;
		SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
		
		//affichage de la bordure haute
		sel_sprite.x = 31;
		sel_sprite.y = 0;
		sel_sprite.w = 40;
		sel_sprite.h = 30;
		
		position.x = e_state->tab[i].position.x + 30; //on ajoute les 30 px de largeur du coin
		position.y = e_state->tab[i].position.y;
		//tant qu'on ne dépasse pas la largeur du message (moins le coin, moins un bandeau inférieur à 40px)
		//on affiche la bordure par tranche de 40 px (voir sprite)
		while( position.x + 40 <= e_state->tab[i].position.x + e_state->tab[i].position.w - 30 ) 
		{
			
			SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
			position.x += 40;
		}	
		//on affiche ce qui reste pour finir la bordure
		sel_sprite.w = (e_state->tab[i].position.w - 60) % 40;
		SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
		
		
		//affichage de la bordure basse (idem bodure haute)
		sel_sprite.x = 31;
		sel_sprite.y = 72;
		sel_sprite.w = 40;
		sel_sprite.h = 30;
		position.x = e_state->tab[i].position.x + 30;
		position.y = e_state->tab[i].position.y + e_state->tab[i].position.h - 30;
		while( position.x + 40 <= e_state->tab[i].position.x + e_state->tab[i].position.w - 30 )
		{
			
			SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
			position.x += 40;
			
		}	
		sel_sprite.w = (e_state->tab[i].position.w - 60) % 40;
		SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
		
		
		//affichage de la bordure gauche (idem bodure haute)
		sel_sprite.x = 0;
		sel_sprite.y = 31;
		sel_sprite.w = 30;
		sel_sprite.h = 40;
		position.x = e_state->tab[i].position.x;
		position.y = e_state->tab[i].position.y + 30;
		while( position.y + 40 <= e_state->tab[i].position.y + e_state->tab[i].position.h - 30 )
		{
			
			SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
			position.y += 40;
			
		}	
		sel_sprite.h = (e_state->tab[i].position.h - 60) % 40;
		SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
		
		
		//affichage de la bordure droite (idem bodure haute)
		sel_sprite.x = 72;
		sel_sprite.y = 31;
		sel_sprite.w = 30;
		sel_sprite.h = 40;
		position.x = e_state->tab[i].position.x + e_state->tab[i].position.w - 30;
		position.y = e_state->tab[i].position.y + 30;
		while( position.y + 40 <= e_state->tab[i].position.y + e_state->tab[i].position.h - 30 )
		{
			
			SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
			position.y += 40;
			
		}	
		sel_sprite.h = (e_state->tab[i].position.h - 60) % 40;
		SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
		
		//affichage du fond
		sel_sprite.x = 31;
		sel_sprite.y = 31;
		sel_sprite.w = 40;
		sel_sprite.h = 40;
		
		position.x = e_state->tab[i].position.x + 30; //on ajoute les 30 px de largeur du coin
		position.y = e_state->tab[i].position.y + 30;
		
		while( position.x + 40 <= e_state->tab[i].position.x + e_state->tab[i].position.w - 30)
		{
			while( position. y + 40 <= e_state->tab[i].position.y + e_state->tab[i].position.h - 30)
			{
				SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
				position.y += 40;
				
			}
			sel_sprite.h = (e_state->tab[i].position.h - 60) % 40;
			SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
			sel_sprite.h = 40;
			position.y = e_state->tab[i].position.y + 30;
			position.x += 40;
			
		}
		
		//on finit de remplir la partie droite
		position.y = e_state->tab[i].position.y + 30;
		while( position. y + 40 <= e_state->tab[i].position.y + e_state->tab[i].position.h - 30)
		{
			sel_sprite.w = (e_state->tab[i].position.w - 60) % 40;
			SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
			position.y += 40;
		}
		
		//on remplit le dernier coin en bas à droite
		sel_sprite.h = (e_state->tab[i].position.h - 60) % 40;
		SDL_BlitSurface(border, &sel_sprite, d_manager->backBuffer, &(position));
		
		
		
		position.y = e_state->tab[i].position.y+40;
		
		for(int j = 0; j < e_state->tab[i].nb_lines; j++)
		{
			text = TTF_RenderUTF8_Blended(d_manager->font, e_state->tab[i].lines[j], noir);
			//pour centrer le texte
			position.x = e_state->tab[i].position.x + (e_state->tab[i].position.w / 2) - text->w/2;
			
			SDL_BlitSurface(text, NULL, d_manager->backBuffer, &(position));
			position.y += 60;
			SDL_FreeSurface(text);
		}
		
	}
}

void free_surface(display_manager* d_manager)
{
    SDL_FreeSurface(d_manager->background);
    SDL_FreeSurface(d_manager->white);
    SDL_FreeSurface(d_manager->black);
	SDL_FreeSurface(d_manager->screen);
    SDL_FreeSurface(d_manager->backBuffer);
	SDL_FreeSurface(d_manager->black_out);
    SDL_FreeSurface(d_manager->white_out);
    SDL_FreeSurface(d_manager->icone_ia);
	SDL_FreeSurface(d_manager->icone_human);
    SDL_FreeSurface(d_manager->dice);
	SDL_FreeSurface(d_manager->message_border);
	SDL_FreeSurface(d_manager->message_border_clicked);
	free(d_manager->path_img);
	
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
	
	//sprite des dés
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "message_border.png");
	d_manager->message_border = IMG_Load(path_img_cp);
	
	//sprite des dés
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "message_border_clicked.png");
	d_manager->message_border_clicked = IMG_Load(path_img_cp);
}



