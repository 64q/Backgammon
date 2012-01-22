// Includes standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

// Includes persos
#include "../include/backgammon.h"
#include "ai.h"
#include "moves.h"

#define AI_MAX_MOVES 50
#define IA_DECISION_WEIGHT_FLOOR 80
#define START_BOARD "205B3G5I205B3G5I"
#define LOCK 1

typedef struct SGameStateAndMoves
{
	SGameState gamestate;
	SMove moves[4];
	int real_length;
} SGameStateAndMoves;

typedef struct SToRegisterFileName
{
	char filename[66];
	EPlayer owner;
} SToRegisterFileName;

// Définition des variables locales
char ai_name[50];

// Si convers_array est modifié, changer START_BOARD en conséquence
char convers_array[28] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R'};

SGameState* ai_game_state;
unsigned int ai_target_score;

SToRegisterFileName* boards = NULL;
int l_boards = 0;
int winner = 0;

// ----------------------------------
// Définition des fonctions locales

// définition du meilleur coup
void ai_register_fn(SToRegisterFileName played_filename);

void ai_best_move(SMove moves[4], SList_moves** poss_moves, int ln_moves, SGameState* gamestate, int lmax_move, SToRegisterFileName* played_filename);

void ai_best_moves(SGameStateAndMoves** pgs, SMove moves[4], int l_gs, SToRegisterFileName* played_filename, int lmax_moves);

void ai_create_file_name(char** file_name, SZone zones[28], EPlayer* player);

int ai_calc_weight(char* filename, EPlayer player);

int ai_comp_zones(SZone zone1[28], SZone zone2[28]);

int add_gm(SGameStateAndMoves** pgm, int l_pgm, SGameStateAndMoves gm);

// Initialise la librairie
void InitLibrary(char name[50])
{
	strcpy(name, "IA de Yann");
	printf("Ghost Busters!\n");
}

// Permet d'initialiser l'IA pour un match
void StartMatch(const unsigned int target_score)
{
	// Init vars
	winner = 0;
	free(boards);
	l_boards = 0;
	ai_target_score = target_score;
}

// Initialiser l'IA pour une partie
void StartGame()
{
	
}

// Fin d'une partie
void EndGame()
{

}

// Fin d'un match
void EndMatch()
{
	int i;
	int victory = 0;
	int defeat = 0;
	
	if (LOCK == 0)
	{
		printf("Ecriture\n");
		FILE* my_file = NULL;
	
		for (i = 0; i < l_boards; i++)
		{
			char absolute_fn[66] = "DB_GS/";
			strcat(absolute_fn, boards[i].filename);
		
			if ((my_file = fopen(absolute_fn, "r")) != NULL)
			{
				fscanf(my_file, "%d;%d", &victory, &defeat);
			}
			if ((my_file = fopen(absolute_fn, "w")) != NULL)
			{
			
				if (boards[i].owner == EPlayer1)
				{
					victory += winner;
					defeat += 1 - winner;
				}
				else
				{
					victory += 1 - winner;
					defeat += winner;				
				}
				
				fprintf(my_file, "%d;%d", victory, defeat);
			}
		
			victory = 0;
			defeat = 0;
		}
		printf("Fin écriture\n");
	}
	winner = 0;
	free(boards);
	l_boards = 0;
}

// Décider si un doublage doit être fait
int DoubleStack(const SGameState * const gameState)
{
	return 0;
}

// Prendre un doublage
int TakeDouble(const SGameState * const gameState)
{
	return 1;
}

void ai_register_fn(SToRegisterFileName played_filename)
{
	if (strcmp(played_filename.filename, "WIN") != 0)
	{
		if (strcmp(played_filename.filename, START_BOARD) != 0)
		{
			if (l_boards == 0)
			{
				boards = (SToRegisterFileName*) malloc(sizeof(SToRegisterFileName));
			}
			else
			{
				boards = (SToRegisterFileName*) realloc(boards, sizeof(SToRegisterFileName) * (l_boards + 1));
			}
			
			boards[l_boards] = played_filename;
			l_boards += 1;
		}
	}
}

// Retourner une liste de mouvements
void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError)
{	
	int i;

	ai_game_state = (SGameState*) gameState; // copie potentiellement modifiable du gameState
	
	EPlayer player;
	char* filename = NULL;
	SToRegisterFileName r_filename;
	
	ai_create_file_name(&filename, ai_game_state->zones, &player);
	
	i = 0;
	while (filename[i] != '\0')
	{
		r_filename.filename[i] = filename[i];
		i++;
	}
	
	r_filename.filename[i] = '\0';
	r_filename.owner = player;
	
	ai_register_fn(r_filename);
	
	SList_moves* possibles_moves = NULL;

	int lm = 0;
	
	int max_lmove = 0; // entier qui indiquera la taille de la plus longue combinaison de coup
	
	//printf("Dé 1 : %i\nDé 2 : %i\n", ai_game_state->die1, ai_game_state->die2); 
	max_lmove = calc_moves(ai_game_state,&possibles_moves,&lm,0,0); // calcule les combinaisons de coups et récupère leur nombre

	lm = clean_possibles_moves(&possibles_moves, lm, max_lmove);
	
	//print_poss_moves(&possibles_moves, lm, max_lmove);
	SToRegisterFileName played_filename;

	ai_best_move(moves, &possibles_moves, lm, ai_game_state, max_lmove, &played_filename);
	//for (i = 0; i < max_lmove; i++)
	//{
	//	printf("Mouvement %i : %i/%i\n", i+1, moves[i].src_point+1, moves[i].dest_point+1);
	//}
	for (i = max_lmove; i < 4; i++)
	{
		moves[i].src_point = -1;
		moves[i].dest_point = -1;
	}
	
	ai_register_fn(played_filename);
	
	if (possibles_moves != NULL)
	{
		free_SList_moves(&possibles_moves, lm, max_lmove);
	}
}

void ai_best_move(SMove moves[4], SList_moves** poss_moves, int ln_moves, SGameState* gamestate, int lmax_moves, SToRegisterFileName* played_filename)
{
	int i,j,k,l;
	SGameStateAndMoves** temp_psgm = (SGameStateAndMoves**) malloc(sizeof(SGameStateAndMoves*));
	int real_length = 0;
	
	if (lmax_moves > 0)
	{
		for (i = 0; i < ln_moves; i++)
		{
			SList_moves* plist_1 = ((*poss_moves) + i);
			if (lmax_moves > 1)
			{
				for (j = 0; j < (plist_1->l_nexts); j++)
				{
					SList_moves* plist_2 = (plist_1->nexts) + j;
					if (lmax_moves > 2)
					{
						for (k = 0; k < (plist_2->l_nexts); k++)
						{
							SList_moves* plist_3 = (plist_2->nexts) + k;
							if (lmax_moves > 3)
							{
								for (l = 0; l < (plist_3->l_nexts); l++)
								{
									SList_moves* plist_4 = (plist_3->nexts) + l;
									SGameStateAndMoves temp_gm;
									
									temp_gm.gamestate = update_game_state(*gamestate, (*plist_1));
									temp_gm.gamestate = update_game_state(temp_gm.gamestate, (*plist_2));
									temp_gm.gamestate = update_game_state(temp_gm.gamestate, (*plist_3));
									temp_gm.gamestate = update_game_state(temp_gm.gamestate, (*plist_4));
									
									temp_gm.real_length = lmax_moves;
									temp_gm.moves[0] = plist_1->head;
									temp_gm.moves[1] = plist_2->head;
									temp_gm.moves[2] = plist_3->head;
									temp_gm.moves[3] = plist_4->head;
									
									real_length = add_gm(temp_psgm, real_length, temp_gm);
								}
							}
							else
							{
								SGameStateAndMoves temp_gm;
								temp_gm.gamestate = update_game_state(*gamestate, (*plist_1));
								temp_gm.gamestate = update_game_state(temp_gm.gamestate, (*plist_2));
								temp_gm.gamestate = update_game_state(temp_gm.gamestate, (*plist_3));
				
								temp_gm.real_length = lmax_moves;
								temp_gm.moves[0] = plist_1->head;
								temp_gm.moves[1] = plist_2->head;
								temp_gm.moves[2] = plist_3->head;
								
								real_length = add_gm(temp_psgm, real_length, temp_gm);
							}
						}
					}
					else
					{
						SGameStateAndMoves temp_gm;
						temp_gm.gamestate = update_game_state(*gamestate, (*plist_1));
						temp_gm.gamestate = update_game_state(temp_gm.gamestate, (*plist_2));
				
						temp_gm.real_length = lmax_moves;
						temp_gm.moves[0] = plist_1->head;
						temp_gm.moves[1] = plist_2->head;
				
						real_length = add_gm(temp_psgm, real_length, temp_gm);
					}
				}
			}
			else
			{
				SGameStateAndMoves temp_gm;
				temp_gm.gamestate = update_game_state(*gamestate, (*plist_1));
				
				temp_gm.real_length = lmax_moves;
				temp_gm.moves[0] = plist_1->head;
				
				real_length = add_gm(temp_psgm, real_length, temp_gm);
			}
		}
		
		ai_best_moves(temp_psgm, moves, real_length, played_filename, lmax_moves);
	}
}

void ai_best_moves(SGameStateAndMoves** pgs, SMove moves[4], int l_gs, SToRegisterFileName* played_filename, int lmax_moves)
{
	int i,j;
	SMove new_moves[4];
	SToRegisterFileName new_pfilename;
	
	SMove file_moves[4];
	SToRegisterFileName file_pfilename;
	
	
	int nm_void = 1;
	int f_void = 1;
	int weight = -1;
	
	for (i = 0; i < l_gs; i++)
	{	
		if ((*((*pgs) + i)).gamestate.zones[EPos_OutP1].nb_checkers == 15)
		{
			winner = 1;
			for (j = 0; j < lmax_moves; j++)
			{
				moves[j] = (*((*pgs) + i)).moves[j];
			}

			(*played_filename).filename[0] = 'W';
			(*played_filename).filename[1] = 'I';
			(*played_filename).filename[2] = 'N';
			(*played_filename).filename[3] = '\0';
			(*played_filename).owner = EPlayer1; 
			break;
		}
		
		EPlayer player = 0;
		char* file_name = NULL;
		

		ai_create_file_name(&file_name, (*((*pgs) + i)).gamestate.zones, &player);
		
		int temp_weight = ai_calc_weight(file_name, player);
		
		if ((temp_weight == -1) && (nm_void == 1))
		{
			j = 0;
			while (file_name[j] != '\0')
			{
				new_pfilename.filename[j] = file_name[j];
				j++;
			}
			
			new_pfilename.filename[j] = '\0';
				
			new_pfilename.owner = player;
		
			for (j = 0; j < (*((*pgs) + i)).real_length; j++)
			{
				new_moves[j] = (*((*pgs) + i)).moves[j];
			}
			nm_void = 0;
		}
		
		else
		{
			if (temp_weight > weight)
			{
				j = 0;
				while (file_name[j] != '\0')
				{
					file_pfilename.filename[j] = file_name[j];
					j++;
				}
				
				file_pfilename.filename[j] = '\0';
				
				file_pfilename.owner = player;
				
				f_void = 0;
				weight = temp_weight;
				for (j = 0; j < (*((*pgs) + i)).real_length; j++)
				{
					file_moves[j] = (*((*pgs) + i)).moves[j];
				}
			}
		}
		
		free(file_name);
	}
	
	if (i == l_gs)
	{
		if ((nm_void == 1) || (weight >= IA_DECISION_WEIGHT_FLOOR))
		{
			i = 0;
			while (file_pfilename.filename[i] != '\0')
			{
				(*played_filename).filename[i] = file_pfilename.filename[i];
				i++;
			}
		
			(*played_filename).filename[i] = '\0';
			(*played_filename).owner = file_pfilename.owner;
		
			for (i = 0; i < lmax_moves; i++)
			{
				moves[i] = file_moves[i];
			}
		}
		else
		{
			i = 0;
			while (new_pfilename.filename[i] != '\0')
			{
				(*played_filename).filename[i] = new_pfilename.filename[i];
				i++;
			}
		
			(*played_filename).filename[i] = '\0';
			(*played_filename).owner = new_pfilename.owner;
			
			for (i = 0; i < lmax_moves; i++)
			{
				moves[i] = new_moves[i];
			}
		}
	}
}

void ai_create_file_name(char** file_name, SZone zones[28], EPlayer* player)
{
	int i,position;
	
	char string_p1[30];
	int pos_1 = 0;
	char string_p2[30];
	int pos_2 = 0;
	
	for (i = EPos_1; i <= EPos_BarP2; i++)
	{
		if (zones[i].nb_checkers != 0)
		{			
			if (zones[i].player == EPlayer1)
			{		
				string_p1[pos_1] = convers_array[zones[i].nb_checkers];
				string_p1[pos_1+1] = convers_array[i];
				pos_1 += 2;			
			}
			
			else
			{
				if (i < EPos_OutP1)
				{
					position = EPos_24 - i;
				}
				else
				{
					position = i - 1;
				}
		
				string_p2[pos_2] = convers_array[zones[i].nb_checkers];
				string_p2[pos_2+1] = convers_array[position];
		
				pos_2 += 2;
			}
		}
	}
	
	*file_name = (char*) malloc(sizeof(char) * (pos_1 + pos_2 + 1));
	
	if (string_p1[1] > string_p2[1])
	{		
		for (i = 0; i < pos_1; i++)
		{
			(*file_name)[i] = string_p1[i];
		}
		
		for (i = 0; i < pos_2; i++)
		{
			(*file_name)[i + pos_1] = string_p2[i];
		}
		
		*player = EPlayer1;
	}
	else
	{
		for (i = 0; i < pos_2; i++)
		{
			(*file_name)[i] = string_p2[i];
		}
		
		for (i = 0; i < pos_1; i++)
		{
			(*file_name)[i + pos_2] = string_p1[i];
		}
		
		*player = EPlayer2;
	}
	
	(*file_name)[pos_1 + pos_2] = '\0';
}

int ai_calc_weight(char* filename, EPlayer player)
{
	char absolute_fn[66] = "DB_GS/";
	strcat(absolute_fn, filename);
	
	int victory = 0;
	int defeat = 0;
	
	FILE* file_gs = NULL;
	file_gs = fopen(absolute_fn, "r");
	if (file_gs != NULL)
	{		
		fscanf(file_gs, "%d;%d", &victory, &defeat);
		
		if (player == EPlayer1)
		{
			return (victory * 100)/(victory + defeat);
		}
		else
		{
			return (defeat * 100)/(victory + defeat);
		}
		
		fclose(file_gs);
	}
	
	return -1;
}

int ai_comp_zones(SZone zone1[28], SZone zone2[28])
{
	int i;
	
	for (i = 0; i < 28; i++)
	{
		if (((zone1[i].nb_checkers != 0) || (zone2[i].nb_checkers != 0)) && ((zone1[i].player != zone2[i].player) || (zone1[i].nb_checkers != zone2[i].nb_checkers)))
		{
			return 0;
		}
	}
	
	return 1;
}

int add_gm(SGameStateAndMoves** pgm, int l_pgm, SGameStateAndMoves gm)
{
	if (l_pgm == 0)
	{
		*pgm = (SGameStateAndMoves*) malloc(sizeof(SGameStateAndMoves));
		(*(*pgm)) = gm;
	}
	else
	{
		int i;
		
		for (i = 0; i < l_pgm; i++)
		{
			if (ai_comp_zones((*((*pgm) + i)).gamestate.zones, gm.gamestate.zones) == 1)
			{
				return l_pgm;
			}
		}
		
		*pgm = (SGameStateAndMoves*) realloc(*pgm, sizeof(SGameStateAndMoves) * (l_pgm + 1));
		*((*pgm) + l_pgm) = gm;
	}
	
	l_pgm += 1;
	return l_pgm;
}