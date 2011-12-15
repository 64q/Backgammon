#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>

#include "../include/display.h"


int main(int argc, char *argv[])
{
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

