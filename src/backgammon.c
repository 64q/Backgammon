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
	

	// Vérification des args de la ligne de commande
	if (argc < 2)
	{
		fprintf(stderr, "usage: %s <lib_path>\n", *(argv));
		//exit(1);
	}


	int fps = 60;
	int delay = 1000/fps;

	display_manager d_manager;
	init_display(&d_manager, "./styles/quentin/");
	
	engine_state e_state;
	init_engine(&e_state,"Erwan", HUMAN, "", "Ordi", IA, "./lib/libai.so");
	

	
	SDL_Event event;

	
	while (e_state.run)
	{
		
		interface_display(&d_manager, &e_state);

		
		Uint8 *keystates = SDL_GetKeyState( NULL );
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					shutdown(&e_state);
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
						shutdown(&e_state);
					}
					
					break;
				case SDL_MOUSEBUTTONDOWN:
					on_click_listener(&e_state, d_manager.ratio);
					break;
				case SDL_MOUSEBUTTONUP:
					on_unclick_listener(&e_state, d_manager.ratio);
					
					break;
			}
		}
		
		SDL_Delay(delay);
		SDL_Flip(d_manager.screen);
	}

	free_surface(&d_manager);

	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}

