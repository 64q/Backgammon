// Includes standards
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

// Includes persos
#include "../include/backgammon.h"
#include "ai.h"
#include "moves.h"


#define IA_DECISION_WEIGHT_FLOOR 80		// seuil sous lequel on tente de jouer un coup inconnu
#define START_BOARD "55375C2N55375C2N"	// filename du plateau de départ qu'on évitera d'enregistrer
#define LOCK 1							// si LOCK = 0 on enregistre à chaque fin de match, sinon non
#define DB_PATH "DB_GS"
// structure utile au retour de paramètres SGameState et SMove[4]

typedef struct
{
	SGameState gamestate;
	SMove moves[4];
	int real_length;
} SGameStateAndMoves;

// structure utile au stockage de paramètre char[66] et EPlayer 
typedef struct
{
	char filename[66];
	EPlayer owner;
} SToRegisterFileName;

typedef union
{
	char car[2];
	int val[2];
} ABR_head;

typedef struct ABR_Stat
{
	ABR_head head;
	int l_nexts;
	struct ABR_Stat* nexts; 
} ABR_Stat;

// Définition des variables locales
char ai_name[50];

// Si convers_array est modifié, changer START_BOARD en conséquence
char convers_array[27] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q'};

SGameState* ai_game_state;
unsigned int ai_target_score;

ABR_Stat *stats = NULL;
int abr_len = 0;

int DB_STATE = -1;
int line = 0;

SToRegisterFileName* boards = NULL;
int l_boards = 0;
int winner = 0;

// ----------------------------------
// Définition des fonctions locales

/* Stocker en mémoire le filename joué
 * @param SToRegisterFileName played_filename
 *  structure contenant le filename à stocker et le joueur "possesseur" du filename
 */
void ai_register_fn(SToRegisterFileName played_filename);

/* Stocker en mémoire le plateau courant
 * @param SGameState* gamestate
 *  structure contenant les informations du plateau qui sera transformé en filename
 */
void ai_register_current_fn(SGameState* gamestate);

/* Obtenir la meilleure combinaisons de coups
 */
void ai_best_move(SMove moves[4], SList_moves** poss_moves, int ln_moves, SGameState* gamestate, int lmax_move, SToRegisterFileName* played_filename);

void ai_best_moves(SGameStateAndMoves** pgs, SMove moves[4], int l_gs, SToRegisterFileName* played_filename, int lmax_moves);

void ai_create_file_name(char** file_name, SZone zones[28], EPlayer* player);

int ai_calc_weight(char* filename, EPlayer player);

int ai_comp_zones(SZone zone1[28], SZone zone2[28]);

void ai_modify_ABR_vd(char *filename, EPlayer owner, int winner, int* victory, int* defeat, int* vd_line);

int add_gm(SGameStateAndMoves** pgm, int l_pgm, SGameStateAndMoves gm);

int ai_seek_ABR(char a, char b, ABR_Stat** p_tab_stats, int len_stats, int no_add);

int ai_add_ABR(char a, char b, ABR_Stat** p_tab_stats, int len_stats);

int ai_add_ABR_end(int a, int b, ABR_Stat** p_tab_stats, int len_stats, int vd_line);

char* ai_int_to_char6(int x);

// Initialise la librairie
void InitLibrary(char name[50])
{
	strcpy(name,"IA de Yann");
}

// Permet d'initialiser l'IA pour un match
void StartMatch(const unsigned int target_score)
{
	// Init vars
	ai_target_score = target_score;
	FILE* db_file = NULL;
	if ((db_file = fopen(DB_PATH, "r")) == NULL)
	{
		DB_STATE = -1;
	}
	else
	{
		char hash[58];
		ABR_Stat* current = NULL;
		int victory,defeat,i,j,pos;
		char string_victory[8];
		char string_defeat[8];
		
		while ((fgets(hash, 57, db_file)) != NULL)
		{
			fgets(string_victory, 7, db_file);
			fgets(string_defeat, 7, db_file);
			
			//printf("hash : %s\n", hash);
			//printf("victory : %s\n", string_victory);
			//printf("defeat : %s\n", string_defeat);
			
			current = NULL;
			victory = 0;
			defeat = 0;
			
			for (i = 0; i < 6; i++)
			{
				for (j = 0; j < 10; j++)
				{
					if (convers_array[j] == string_victory[i])
					{
						victory += j*((int) pow(10,(5-i)));
					}
					if (convers_array[j] == string_defeat[i])
					{
						defeat += j*((int) pow(10,(5-i)));
					}
				}
			}
			
			//printf("victory après : %i\n", victory);
			
			for (i = 0; i < 28; i++)
			{
				if (hash[i*2] != '0')
				{
					if (current == NULL)
					{
						pos = ai_seek_ABR(hash[i*2], hash[(i*2)+1], &stats, abr_len, 0);
						if (pos == abr_len)
						{
							abr_len += 1;
						}
						
						current = (stats + pos);
					}
					else
					{
						pos = ai_seek_ABR(hash[i*2], hash[(i*2)+1], (&(current->nexts)), current->l_nexts, 0);
						if (pos == current->l_nexts)
						{
							current->l_nexts += 1;
						}
						
						current = (ABR_Stat*) ((current->nexts) + pos);
					}
				}
			}
			
			ai_add_ABR_end(victory, defeat, (&(current->nexts)), current->l_nexts, line);
			current->l_nexts += 1;
			line += 1;
		}
		
		DB_STATE = 1;
		fclose(db_file);
	}
}

// Initialiser l'IA pour une partie
void StartGame()
{
	winner = 0;
	free(boards);
	boards = NULL;
	l_boards = 0;	
}

// Fin d'une partie
void EndGame()
{
	int i, j, len, file_descriptor, victory, defeat, vd_line;
	int decalage;
	char buffer[70];
	char string0[50];
	char* value = NULL;
	
	if ((LOCK == 0))
	{
		if ((file_descriptor = open(DB_PATH, O_CREAT|O_WRONLY, S_IRWXU|S_IRWXG|S_IRWXO)) != -1)
		{
			for (i = 0; i < l_boards; i++)
			{
				ai_modify_ABR_vd(boards[i].filename, boards[i].owner, winner, &victory, &defeat, &vd_line);
				if (vd_line == line)
				{
					line += 1;
					len = strlen(boards[i].filename);
					
					string0[0]='\0';

					for (j = 0; j < ((56 - len)/2); j++)
					{
						strcat(string0, "00");
					}
					
					//printf("hash : %s, v : %i, d : %i\n", boards[i].filename, victory, defeat);
					sprintf(buffer, "%s%s00000%i00000%i", string0, boards[i].filename, victory, defeat);
					
					if(buffer[0] == '0')
					{
						lseek(file_descriptor, 0, SEEK_END);
						write(file_descriptor, buffer, 68);
					}
					else
					{
						printf("hash : %s\n", boards[i].filename);
						printf("buffer : %s\n", buffer);
					}
				}
				else
				{
					if (((boards[i].owner == EPlayer1) && (winner == 1)) || ((boards[i].owner == EPlayer2) && (winner == 0)))
					{
						decalage = 0;
						value = ai_int_to_char6(victory);
					}
					else
					{
						decalage = 6;
						value = ai_int_to_char6(defeat);
					}
					
					lseek(file_descriptor, ((68*vd_line) + 56 + decalage), SEEK_SET);
					write(file_descriptor, value, 6);
					
					free(value);
					value = NULL;
				}
			}
			
			close(file_descriptor);
			DB_STATE = 1;
		}
	}
	
	else
	{
		for (i = 0; i < l_boards; i++)
		{
			ai_modify_ABR_vd(boards[i].filename, boards[i].owner, winner, &victory, &defeat, &vd_line);
			//printf("hash : %s, v : %i, d : %i\n", boards[i].filename, victory, defeat);
		}
	}
	
	winner = 0;
	free(boards);
	boards = NULL;
	l_boards = 0;
}

// Fin d'un match
void EndMatch()
{
	if (DB_STATE == 1)
	{
		// freeABR_Stat(stats, abr_len);
	}
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
			if((strlen(played_filename.filename)%2 != 0) || (strlen(played_filename.filename) == 0))
			{
				printf("%s\n", played_filename.filename);
			}
			else
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
}

void ai_register_current_fn(SGameState* gamestate)
{
	int i;
	
	EPlayer player;
	char* filename = NULL;
	int len = 0;
	SToRegisterFileName r_filename;
		
	ai_create_file_name(&filename, ai_game_state->zones, &player);
	len = strlen(filename);
	
	int sym = 1;
	for (i = 0; i < (len/2); i++)
	{
		if (filename[i] != filename[i + (len/2)])
		{
			sym = 0;
			break;
		}
	}
	
	if (sym == 1)
	{
		if (player == EPlayer1)
		{
			player = EPlayer2;
		}
		else
		{
			player = EPlayer1;
		}
	}
	
	i = 0;
	while (filename[i] != '\0')
	{
		r_filename.filename[i] = filename[i];
		i++;
	}
	
	r_filename.filename[i] = '\0';
	r_filename.owner = player;
	
	ai_register_fn(r_filename);
	
	free(filename);
}

// Retourner une liste de mouvements
void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError)
{	
	ai_game_state = (SGameState*) gameState; // copie potentiellement modifiable du gameState
	
	ai_register_current_fn(ai_game_state);
	
	SList_moves* possibles_moves = NULL;
	int lm = 0;
	int max_lmove = 0; // entier qui indiquera la taille de la plus longue combinaison de coup
	
	max_lmove = calc_moves(ai_game_state,&possibles_moves,&lm,0,0); // calcule les combinaisons de coups et récupère la taille de la plus grande
	lm = clean_possibles_moves(&possibles_moves, lm, max_lmove);
	
	SToRegisterFileName played_filename;
	
	if (max_lmove != 0)
	{
		ai_best_move(moves, &possibles_moves, lm, ai_game_state, max_lmove, &played_filename);
		ai_register_fn(played_filename);
		free_SList_moves(&possibles_moves, lm, max_lmove);
	}
}

void ai_best_move(SMove moves[4], SList_moves** poss_moves, int ln_moves, SGameState* gamestate, int lmax_moves, SToRegisterFileName* played_filename)
{
	int i,j,k,l;
	SGameStateAndMoves* temp_psgm = NULL;
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
									
									real_length = add_gm(&temp_psgm, real_length, temp_gm);
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
								
								real_length = add_gm(&temp_psgm, real_length, temp_gm);
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
				
						real_length = add_gm(&temp_psgm, real_length, temp_gm);
					}
				}
			}
			else
			{
				SGameStateAndMoves temp_gm;
				temp_gm.gamestate = update_game_state(*gamestate, (*plist_1));
				
				temp_gm.real_length = lmax_moves;
				temp_gm.moves[0] = plist_1->head;
				
				real_length = add_gm(&temp_psgm, real_length, temp_gm);
			}
		}
		
		ai_best_moves(&temp_psgm, moves, real_length, played_filename, lmax_moves);
	}
	
	free(temp_psgm);
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
		//printf("weight : %i\n", temp_weight);
		
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
		}
		
		if (i < EPos_OutP1)
		{
			position = EPos_24 - i;
		}
		else
		{
			position = i;
		}
		
		if (zones[position].nb_checkers != 0)
		{
			if (zones[position].player == EPlayer2)
			{			
				string_p2[pos_2] = convers_array[zones[position].nb_checkers];
				
				if (position == i)
				{
					position -= 2;
				}
				else
				{
					position = i;
				}
				
				string_p2[pos_2+1] = convers_array[position];
		
				pos_2 += 2;
			}
		}
	}
	
	*file_name = (char*) malloc(sizeof(char) * (pos_1 + pos_2 + 1));
	
	if (strcmp(string_p1, string_p2) > 0)
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
	int len = strlen(filename);
	int i, pos;
	int absent = 0;
	ABR_Stat* current = NULL;
	
	if (DB_STATE == 1)
	{
		if ((len%2) == 0)
		{
			for (i = 0; i < (len/2); i++)
			{
				if (current == NULL)
				{
					pos = ai_seek_ABR(filename[i*2], filename[(i*2)+1], &stats, abr_len, 1);
					if (pos == abr_len)
					{
						absent = 1;
						break;
					}
				
					current = (stats + pos);
				}
				else
				{
					pos = ai_seek_ABR(filename[i*2], filename[(i*2)+1], (&(current->nexts)), current->l_nexts, 1);
					if (pos == current->l_nexts)
					{
						absent = 1;
						break;
					}
				
					current = (ABR_Stat*) ((current->nexts) + pos);
				}
			}
		}

		if (absent == 0)
		{
			int v = current->nexts->head.val[0];
			int d = current->nexts->head.val[1];
			
			
			if (player == EPlayer1)
			{
				//printf("%i\n", ((v*100)/(v+d)));
				return ((v*100)/(v+d));
			}
			else
			{
				//printf("%i\n", ((d*100)/(v+d)));
				return ((d*100)/(v+d));
			}
		}
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

void ai_modify_ABR_vd(char *filename, EPlayer owner, int winner, int* victory, int* defeat, int* vd_line)
{
	int i, pos, add;
	int len = strlen(filename);
	ABR_Stat* current = NULL;
	
	for (i = 0; i < (len/2); i++)
	{
		if (i == 0)
		{
			pos = ai_seek_ABR(filename[i*2], filename[(i*2)+1], &stats, abr_len, 0);
			if (pos == abr_len)
			{
				abr_len += 1;
			}
			
			current = (stats + pos);
		}
		else
		{
			pos = ai_seek_ABR(filename[i*2], filename[(i*2)+1], &(current->nexts), current->l_nexts,0);
			if (pos == current->l_nexts)
			{
				current->l_nexts += 1;
			}

			current = ((current->nexts) + pos);
		}	
	}
	 
	if (owner == EPlayer1)
	{
		add = winner;
	}
	else
	{
		add = 1 - winner;
	}
	
	if (current->l_nexts == 1)
	{	
		current->nexts->head.val[0] += add;
		current->nexts->head.val[1] += (1-add);
		*victory = current->nexts->head.val[0];
		*defeat = current->nexts->head.val[1];
		*vd_line = current->nexts->l_nexts;
	}
	else
	{	
		*victory = add;
		*defeat = (1-add);
		*vd_line = line;
		current->l_nexts = ai_add_ABR_end(*victory, *defeat, &(current->nexts), 0, line);
	}
}

int ai_seek_ABR(char a, char b, ABR_Stat** p_tab_stats, int len_stats, int no_add)
{
	int i;

	for (i = 0; i < len_stats; i++)
	{
		if (((*p_tab_stats)[i].head.car[0] == a) && ((*p_tab_stats)[i].head.car[1] == b))
		{
			return i;
		}
	}
	
	if (no_add == 0)
	{
		len_stats = ai_add_ABR(a, b, p_tab_stats, len_stats);
	}
	
	return (len_stats - (1 - no_add));
}

int ai_add_ABR(char a, char b, ABR_Stat** p_tab_stats, int len_stats)
{
	if (len_stats == 0)
	{
		*p_tab_stats = (ABR_Stat*) malloc(sizeof(ABR_Stat));
	}
	else
	{
		*p_tab_stats = (ABR_Stat*) realloc(*p_tab_stats, sizeof(ABR_Stat) * (len_stats + 1));
	}
	
	ABR_Stat temp;
	temp.head.car[0] = a;
	temp.head.car[1] = b;
	temp.l_nexts = 0;
	temp.nexts = NULL;
	(*p_tab_stats)[len_stats] = temp;
	
	return (len_stats + 1);
}

int ai_add_ABR_end(int a, int b, ABR_Stat** p_tab_stats, int len_stats, int vd_line)
{
	if (len_stats == 0)
	{
		*p_tab_stats = (ABR_Stat*) malloc(sizeof(ABR_Stat));
	}
	else
	{
		printf("Plus d'une feuille! O_o\n");
	}
	
	ABR_Stat temp;
	temp.head.val[0] = a;
	temp.head.val[1] = b;
	temp.l_nexts = vd_line;
	temp.nexts = NULL;
	(*p_tab_stats)[0] = temp;
	
	return 1;
}

char* ai_int_to_char6(int x)
{
	int i,j;
	char* res = (char*) malloc(sizeof(char) * 7);
	res[0] = res[1] = res[2] = res[3] = res[4] = res[5] = '0';
	res[6] = '\0';
	
	for (i = 1; (x%((int) pow(10,i))) != x; i++);
	
	for (j = 1; j <= i; j++)
	{
		res[6-j] = convers_array[((x%((int) pow(10,j)))/((int) pow(10,j-1)))];
	}
	
	//printf ("%i : %s\n", x, res);
	
	return res;
}
