// Includes standards
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dlfcn.h>
// Includes SDL
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
// Includes persos
#include "../include/engine.h"
#include "../include/display.h"
#include "../lib/ai.h"

int main(int argc, char *argv[])
{
	void *lib;

	// Vérification des args de la ligne de commande
<<<<<<< HEAD
	if (argc < 1) {
=======
	if (argc < 2)
	{
>>>>>>> 934ef1a350dcd7a01c1a9b8b67c8c988c314b0cd
		fprintf(stderr, "usage: %s <lib_path>\n", *(argv));
		//exit(1);
	}

	// Ouverture dynamique de la bibliothèque
	if ((lib = dlopen(*(argv + 1), RTLD_LAZY)) == NULL)
	{
		fprintf(stderr, "Error: %s\n", dlerror());
		//exit(1);
	}
<<<<<<< HEAD
	int fps = 60;
	int delay = 1000/fps;
	
	display_manager d_manager;
	
    init_display(&d_manager, "./img/defaut/");
	
	SGameState g_state;
	init_game(&g_state);
	
	player_infos p_infos;
	init_player(&p_infos, "Erwan", HUMAN, "Ordi", IA);
	
	//TEST
	g_state.score = 3;
	g_state.scoreP2 = 2;
	g_state.zones[EPos_BarP1].nb_checkers=3;
	g_state.zones[EPos_BarP2].nb_checkers=4;
	g_state.die1 = 3;
	g_state.die2 = 5;
	
	
    int continuer = 1;
    SDL_Event event;
	
	
    while (continuer)
    {
    	interface_display(&d_manager, &g_state, &p_infos);
		
		
		
        SDL_PollEvent(&event);
        Uint8 *keystates = SDL_GetKeyState( NULL );
		
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
            case SDL_KEYDOWN:
            	
            	if( keystates[ SDLK_LEFT ] ) { 
            		switch_to_full_screen(&d_manager);
				}
				if( keystates[ SDLK_RIGHT ] ) { 
					 switch_to_window(&d_manager);
				}
				if( keystates[ SDLK_UP ] ) { 
					continuer = 0;
				}

            	
        }
		
		SDL_Delay(delay);
        SDL_Flip(d_manager.screen);
    }

    free_surface(&d_manager);
	
	TTF_Quit();
    SDL_Quit();
=======

	display_manager d_manager;

	init_display(&d_manager, "./styles/default/");
	int run = 1;
	SDL_Event event;

	while (run)
	{
	interface_display(&d_manager);

	SDL_PollEvent(&event);
	Uint8 *keystates = SDL_GetKeyState( NULL );

	switch(event.type)
	{
		case SDL_QUIT:
			run = 0;
		break;
		case SDL_KEYDOWN:
			if (keystates[SDLK_LEFT])
			{
				switch_to_full_screen(&d_manager);
			}
			if (keystates[SDLK_RIGHT])
			{
				switch_to_window(&d_manager);
			}
			if (keystates[SDLK_UP])
			{
				run = 0;
			}
		break;
	}

	SDL_Flip(d_manager.screen);
	}
>>>>>>> 934ef1a350dcd7a01c1a9b8b67c8c988c314b0cd

	free_surface(&d_manager);

	SDL_Quit();

	return EXIT_SUCCESS;
}

