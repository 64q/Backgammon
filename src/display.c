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

void init_display(display_manager* d_manager ,char* name_style)
{
	//position du fond
	d_manager->background_position.x = 0;
	d_manager->background_position.y = 0;
	
	d_manager->moving_checker_pos.x = -1;
	d_manager->moving_checker_pos.y = -1;
	
	//intitialistion nb de messages
	d_manager->nb_messages_surface = 0;

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
	char tmp[100];
	sprintf(tmp, "./styles/%s/", name_style);
	
	
	d_manager->path_img = (char*)malloc((strlen(tmp) + 1) * sizeof(char));

	//contiendra la chaîne concaténée
	strcpy(d_manager->path_img, tmp);
	char path_img_cp[100];

	//on charge chaque image du dossier passé en paramètre
	
	
	/* Chargement de la police */
	strcpy(path_img_cp, tmp);
	strcat(path_img_cp, "font.ttf");
	d_manager->font = TTF_OpenFont(path_img_cp, 70);
	if( d_manager->font == NULL)
	{
		printf("Le style n'a pas été défini ou n'existe pas, style par défaut chargé\n");
		d_manager->path_img = "./styles/default/";
		strcpy(tmp,"./styles/default/");
		strcpy(path_img_cp, tmp);
		strcat(path_img_cp, "font.ttf");
		d_manager->font = TTF_OpenFont(path_img_cp, 70);
		
	}
	

	//icone
	strcpy(path_img_cp, tmp);
	strcat(path_img_cp, "icone.png");
	
	
	SDL_WM_SetIcon(IMG_Load(path_img_cp), NULL);

	
	
    
	
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
	SDL_Rect pos;
	pos.x = 0;
	pos.y = 0;
	SDL_BlitSurface(d_manager->background, NULL, d_manager->backBuffer, &(pos));
	
	if(e_state->is_human_playing == true)
	{
		highlight_possible_moves(d_manager, e_state);
	}
	
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
	
	//mise
	stake_display(d_manager, e_state);
	
	if(e_state->is_human_playing && e_state->src_selected_checker != -1)
	{
		moving_checker_display(d_manager, e_state);
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
	
	for(unsigned int i = 0; i < g_state->zones[EPos_OutP2].nb_checkers; i++)
	{
		checker_position.y = 470 - i * 15;
		SDL_BlitSurface(d_manager->black_out, NULL, d_manager->backBuffer, &(checker_position));
	}
	
	for(unsigned int i = 0; i < g_state->zones[EPos_OutP1].nb_checkers; i++)
	{
		checker_position.y = 1030 - i * 15;
		SDL_BlitSurface(d_manager->white_out, NULL, d_manager->backBuffer, &(checker_position));
	}
	
	//affichage des pions de la barre du joueur blanc (donc les pions noirs)
	checker_position.y = 244;
	for(unsigned int i = 0; i < g_state->zones[EPos_BarP1].nb_checkers; i++)
	{
		checker_position.x = 1455 + i * 100;
		SDL_BlitSurface(d_manager->white, NULL, d_manager->backBuffer, &(checker_position));
	}
	//inverse
	checker_position.y = 959;
	for(unsigned int i = 0; i < g_state->zones[EPos_BarP2].nb_checkers; i++)
	{
		checker_position.x = 1455 + i * 100;
		SDL_BlitSurface(d_manager->black, NULL, d_manager->backBuffer, &(checker_position));
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
	
	update_message_surface(d_manager, e_state);
	
	
	SDL_Rect position;
	for(int i = 0; i < d_manager->nb_messages_surface; i++)
	{
		
		position.x = e_state->tab[i].position.x;
		position.y = e_state->tab[i].position.y;
		//printf("S%i\n",position.y);
		if(e_state->tab[i].is_clicked)
		{
			SDL_BlitSurface(d_manager->messages_clicked_surface[i], NULL, d_manager->backBuffer, &(position));
		}else
		{
			SDL_BlitSurface(d_manager->messages_surface[i], NULL, d_manager->backBuffer, &(position));
		}
	}

}

void stake_display(display_manager *d_manager, engine_state* e_state)
{
	SDL_Rect pos;
	pos.x = 0;
	pos.y = 0;
	if(e_state->stake_owner == EPlayer1 + EPlayer2)
	{
		pos.x = 613;
		pos.y = 508;
	}
	else if(e_state->stake_owner == EPlayer1)
	{
		pos.x = 613;
		pos.y = 56;
	}
	else
	{
		pos.x = 613;
		pos.y = 960;
	}
	
	SDL_BlitSurface(d_manager->stake, NULL, d_manager->backBuffer, &pos);
	char stake_str[6];
	SDL_Color noir;
	noir.r = 0;
	noir.g = 0;
	noir.b = 0;
	
	
	int stake_tmp = e_state->g_state.stake;
	if(e_state->g_state.stake == 1)
		stake_tmp = 64;
	
		sprintf(stake_str, "%i", stake_tmp);
	SDL_Surface * texte = TTF_RenderText_Blended(d_manager->font,stake_str , noir );
	pos.x = 645 - texte->w / 2;
	pos.y = pos.y + 32 - texte->h / 2;
	SDL_BlitSurface(texte, NULL, d_manager->backBuffer, &(pos));
}

void highlight_possible_moves(display_manager *d_manager, engine_state* e_state)
{
	SDL_Rect pos;
	int nb_pos;
	int size_tab;
	
	
	if(e_state->src_selected_checker == -1)
	{
		size_tab = e_state->nb_current_possible_moves;
		
	}
	else
	{
		size_tab = e_state->nb_possible_destinations;
	}
	for(int i = 0; i < size_tab; i++)
	{
		if(e_state->src_selected_checker == -1)
		{
			nb_pos = e_state->current_possible_moves[i].head.src_point;
		}
		else
		{
			nb_pos = e_state->possible_destination[i];
		}
		
		if(nb_pos <= EPos_24)
		{
			if(nb_pos <= EPos_6)
			{
				pos.x = 1180 - nb_pos * 100;
			}
			else if(nb_pos <= EPos_12)
			{ 
				pos.x = 510 - (nb_pos - 6) * 100;
			}
			else if( nb_pos <= EPos_18 )
			{
				pos.x = 510 - (EPos_18 - nb_pos) * 100;
			}
			else
			{
				pos.x = 1180 - (EPos_24 - nb_pos) * 100;
			}
			
			if(nb_pos <= EPos_12)
			{
				pos.y = 570;
				SDL_BlitSurface(d_manager->highlight_down, NULL, d_manager->backBuffer, &(pos));
			}
			else
			{
				pos.y = 10;
				SDL_BlitSurface(d_manager->highlight_up, NULL, d_manager->backBuffer, &(pos));
								
			}
		}
		else
		{
			if(nb_pos == EPos_OutP1)
			{
				pos.x = 1290;
				pos.y = 595;
				SDL_BlitSurface(d_manager->highlight_out, NULL, d_manager->backBuffer, &(pos));
			}
			
			else if(nb_pos == EPos_OutP2)
			{
				pos.x = 1290;
				pos.y = 35;
				SDL_BlitSurface(d_manager->highlight_out, NULL, d_manager->backBuffer, &(pos));
			}
			else if(nb_pos == EPos_BarP1)
			{
				pos.x = 1455;
				pos.y = 244;
				SDL_BlitSurface(d_manager->highlight_bar, NULL, d_manager->backBuffer, &(pos));
			}
			else if(nb_pos == EPos_BarP2)
			{
				pos.x = 1455;
				pos.y = 959;
				SDL_BlitSurface(d_manager->highlight_bar, NULL, d_manager->backBuffer, &(pos));
			}
		}
		
	}
}

void moving_checker_display(display_manager* d_manager, engine_state* e_state)
{
	SDL_Rect pos;
	int x, y;
	SDL_GetMouseState(&(x), &(y)); 
	pos.x = (x - 25)/d_manager->ratio;
	pos.y = (y - 25)/d_manager->ratio;
	
	
	if(e_state->g_state.zones[e_state->src_selected_checker].player == EPlayer1)
		SDL_BlitSurface(d_manager->white, NULL, d_manager->backBuffer, &(pos));
		
	if(e_state->g_state.zones[e_state->src_selected_checker].player == EPlayer2)
		SDL_BlitSurface(d_manager->black, NULL, d_manager->backBuffer, &(pos));
}
void update_message_surface(display_manager* d_manager, engine_state* e_state)
{
	int width, height;
	char** text;
	int nb_lines;
	if(e_state->message_load == false)
	{
		d_manager->nb_messages_surface = 0;
	}
	while(d_manager->nb_messages_surface < e_state->nb_messages)
	{
		width = e_state->tab[d_manager->nb_messages_surface].position.w;
		height = e_state->tab[d_manager->nb_messages_surface].position.h;
		text = e_state->tab[d_manager->nb_messages_surface].lines;
		nb_lines = e_state->tab[d_manager->nb_messages_surface].nb_lines;
		
		
		d_manager->messages_surface[d_manager->nb_messages_surface ] = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
		d_manager->messages_clicked_surface[d_manager->nb_messages_surface ] = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);

		create_message_surface(d_manager, d_manager->messages_surface[d_manager->nb_messages_surface ], d_manager->message_border, text, nb_lines, width, height);
		create_message_surface(d_manager, d_manager->messages_clicked_surface[d_manager->nb_messages_surface ], d_manager->message_border_clicked, text, nb_lines, width, height);
	
		d_manager->nb_messages_surface++;
	}
	
	while(d_manager->nb_messages_surface > e_state->nb_messages)
	{
		
		SDL_FreeSurface(d_manager->messages_surface[d_manager->nb_messages_surface - 1]);
		SDL_FreeSurface(d_manager->messages_clicked_surface[d_manager->nb_messages_surface -1]);
		d_manager->nb_messages_surface--;

	}
	
	e_state->message_load = true;
}

void create_message_surface(display_manager* d_manager, SDL_Surface* cible, SDL_Surface* border, char** text, int nb_lines, int width, int height)
{
	SDL_Surface *text_surface;
	
	SDL_Rect sel_sprite;
	SDL_Rect position;
	
	SDL_Color noir;
	noir.r = 0;
	noir.g = 0;
	noir.b = 0;
	
	//affichage du coin haut gauche
	sel_sprite.x = 0;
	sel_sprite.y = 0;
	sel_sprite.w = 30;
	sel_sprite.h = 30;
	position.x = 0;
	position.y = 0;
	
	SDL_BlitSurface(border, &sel_sprite, cible, &(position));
	
	//affichage du coin haut droit
	sel_sprite.x = 72;
	sel_sprite.y = 0;
	sel_sprite.w = 30;
	sel_sprite.h = 30;
	position.x = width - 30;
	SDL_BlitSurface(border, &sel_sprite, cible, &(position));
	
	//affichage du coin bas gauche
	sel_sprite.x = 0;
	sel_sprite.y = 72;
	sel_sprite.w = 30;
	sel_sprite.h = 30;
	position.x = 0;
	position.y = height - 30;
	SDL_BlitSurface(border, &sel_sprite, cible, &(position));
	
	//affichage du coin bas droit
	sel_sprite.x = 72;
	sel_sprite.y = 72;
	sel_sprite.w = 30;
	sel_sprite.h = 30;
	position.x = width - 30;
	position.y = height - 30;
	SDL_BlitSurface(border, &sel_sprite, cible, &(position));
	
	//affichage de la bordure haute
	sel_sprite.x = 31;
	sel_sprite.y = 0;
	sel_sprite.w = 40;
	sel_sprite.h = 30;
	
	position.x = 30; //on ajoute les 30 px de largeur du coin
	position.y = 0;
	//tant qu'on ne dépasse pas la largeur du message (moins le coin, moins un bandeau inférieur à 40px)
	//on affiche la bordure par tranche de 40 px (voir sprite)
	while( position.x + 40 <= 0 + width - 30 ) 
	{
		
		SDL_BlitSurface(border, &sel_sprite, cible, &(position));
		position.x += 40;
	}	
	//on affiche ce qui reste pour finir la bordure
	sel_sprite.w = (width - 60) % 40;
	SDL_BlitSurface(border, &sel_sprite, cible, &(position));
	
	
	//affichage de la bordure basse (idem bodure haute)
	sel_sprite.x = 31;
	sel_sprite.y = 72;
	sel_sprite.w = 40;
	sel_sprite.h = 30;
	position.x = 0 + 30;
	position.y = 0 + height - 30;
	while( position.x + 40 <= 0 + width - 30 )
	{
		
		SDL_BlitSurface(border, &sel_sprite, cible, &(position));
		position.x += 40;
		
	}	
	sel_sprite.w = (width - 60) % 40;
	SDL_BlitSurface(border, &sel_sprite, cible, &(position));
	
	
	//affichage de la bordure gauche (idem bodure haute)
	sel_sprite.x = 0;
	sel_sprite.y = 31;
	sel_sprite.w = 30;
	sel_sprite.h = 40;
	position.x = 0;
	position.y = 30;
	while( position.y + 40 <= 0 + height - 30 )
	{
		
		SDL_BlitSurface(border, &sel_sprite, cible, &(position));
		position.y += 40;
		
	}	
	sel_sprite.h = (height - 60) % 40;
	SDL_BlitSurface(border, &sel_sprite, cible, &(position));
	
	
	//affichage de la bordure droite (idem bodure haute)
	sel_sprite.x = 72;
	sel_sprite.y = 31;
	sel_sprite.w = 30;
	sel_sprite.h = 40;
	position.x = width - 30;
	position.y = 30;
	while( position.y + 40 <= 0 + height - 30 )
	{
		
		SDL_BlitSurface(border, &sel_sprite, cible, &(position));
		position.y += 40;
		
	}	
	sel_sprite.h = (height - 60) % 40;
	SDL_BlitSurface(border, &sel_sprite, cible, &(position));
	
	//affichage du fond
	sel_sprite.x = 31;
	sel_sprite.y = 31;
	sel_sprite.w = 40;
	sel_sprite.h = 40;
	
	position.x = 30; //on ajoute les 30 px de largeur du coin
	position.y = 30;
	
	while( position.x + 40 <= 0 + width - 30)
	{
		while( position. y + 40 <= 0 + height - 30)
		{
			SDL_BlitSurface(border, &sel_sprite, cible, &(position));
			position.y += 40;
			
		}
		sel_sprite.h = (height - 60) % 40;
		SDL_BlitSurface(border, &sel_sprite, cible, &(position));
		sel_sprite.h = 40;
		position.y = 30;
		position.x += 40;
		
	}
	
	//on finit de remplir la partie droite
	position.y = 0 + 30;
	while( position. y + 40 <= 0 + height - 30)
	{
		sel_sprite.w = (width - 60) % 40;
		SDL_BlitSurface(border, &sel_sprite, cible, &(position));
		position.y += 40;
	}
	
	//on remplit le dernier coin en bas à droite
	sel_sprite.h = (height - 60) % 40;
	SDL_BlitSurface(border, &sel_sprite, cible, &(position));
	
	
	
	position.y = 40;
	
	for(int j = 0; j < nb_lines; j++)
	{
		text_surface = TTF_RenderUTF8_Blended(d_manager->font, text[j], noir);
		//pour centrer le texte
		position.x = (width / 2) - text_surface->w/2;
		
		SDL_BlitSurface(text_surface, NULL, cible, &(position));
		position.y += 60;
		SDL_FreeSurface(text_surface);
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
}

void switch_to_full_screen(display_manager* d_manager)
{
	if(d_manager->display_mode != FULL_SCREEN)
	{
		d_manager->ratio = (double)d_manager->res_max[0]/(double)d_manager->background->w;
		d_manager->screen = SDL_SetVideoMode(d_manager->res_max[0], d_manager->res_max[1], 32, SDL_FULLSCREEN|SDL_DOUBLEBUF);
		d_manager->display_mode = FULL_SCREEN;
		d_manager->background_position.y = (int) (d_manager->res_max[1]/2.0 - (d_manager->res_max[0]*9.0/16.0)/2.0);
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
		d_manager->background_position.y = 0;
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
	
	//sprite des de la mise
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "stake.png");
	d_manager->stake = IMG_Load(path_img_cp);
	
	//sprite de la bordure des messsages
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "message_border.png");
	d_manager->message_border = IMG_Load(path_img_cp);
	
	
	//sprite des messages cliqués
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "message_border_clicked.png");
	d_manager->message_border_clicked = IMG_Load(path_img_cp);
	
	//sprite de la mise en valeur des zones d'atterissage possible des pions
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "highlight.png");
	d_manager->highlight_down = IMG_Load(path_img_cp);
	d_manager->highlight_up = rotozoomSurface (d_manager->highlight_down, 180.0, 1.0, 1.0);
	
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "highlight_out.png");
	d_manager->highlight_out = IMG_Load(path_img_cp);
	
	strcpy(path_img_cp, d_manager->path_img);
	strcat(path_img_cp, "highlight_bar.png");
	d_manager->highlight_bar = IMG_Load(path_img_cp);
}



