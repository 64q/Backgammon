// Includes standards
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dlfcn.h>
// Includes persos
#include "../include/engine.h"
#include "../lib/game_ai.h"

int main(int argc, char *argv[])
{
	void *lib;
	typedef void (*fptr_AI_start_match)(int);
	fptr_AI_start_match AI_start_match;

	if ((lib = dlopen("libgame_ai.so", RTLD_LAZY)) == NULL)
	{
		printf("Erreur d'ouverture : %s\n", dlerror());
		return 1;
	}
	if ((AI_start_match=(fptr_AI_start_match)dlsym(lib, "start_match")) == NULL)
	{
		printf("Erreur de lecture");
		return 1;
	}

	AI_start_match(10);

	dlclose(lib);

	return 0;
}

