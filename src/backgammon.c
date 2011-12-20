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
	if (argc < 2) {
		fprintf(stderr, "usage: %s <lib_path>\n", *(argv));
		exit(1);
	}
	
	// Ouverture dynamique de la bibliothèque
	if ((lib = dlopen(*(argv + 1), RTLD_LAZY)) == NULL)
	{
		fprintf(stderr, "Error: %s\n", dlerror());
		exit(1);
	}
	
	display_manager d_manager;
	
    init_display(&d_manager, "./img/defaut/");
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
    	interface_display(&d_manager);
		
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

        SDL_Flip(d_manager.screen);
    }

    free_surface(&d_manager);

    SDL_Quit();

    return EXIT_SUCCESS;
}


