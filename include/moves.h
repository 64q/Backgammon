#ifndef __MOVES_H__
#define __MOVES_H__

#include "./backgammon.h"


typedef struct SList_moves
{
	SMove head; // mouvement de tête
	int l_nexts; // taille de nexts
	struct SList_moves* nexts; // tableau de SList_moves correspondant aux coups suivants
	
	int pmoves; // valeur du dé encore jouable
} SList_moves;

int u();
// fonction calculant les différentes combinaisons de coups possibles
int calc_moves(SGameState* gameState, SList_moves** possibles_moves, int* lp_moves, int move_order, int move_order_limit);

int init_combi_moves(SGameState* gameState, SList_moves** poss_moves);

// fonction calculant les différents mouvements possibles
int calc_primary_moves(SGameState* gameState, SMove** new_moves);

// fonction validant un mouvement sur le tableau (et l'ajoutant si nécessaire)
int from_board(SMove** new_moves, int ln_moves, SZone zones[], int src_point, int die, int out_ok);

// fonction permettant de compléter un tableau de liste comportant des mouvements "primaires"
int complete_list(SGameState gameState, SList_moves* plist, int move_order);

// fonction retourtant un gameState modifié par rapport à une SList_moves
SGameState update_game_state(SGameState gameState, SList_moves list);

// fonction ajoutant un mouvement à un tableau de mouvements
int add_move(SMove** moves, int lmoves, SMove move);

// fonction ajoutant un tableau de mouvement à un tableau de liste (création de nouvelle liste par mouvement)
int add_list_moves(SList_moves** moves, int lmoves);

// fonction supprimant de poss_moves les combinaisons "illegales"
int clean_possibles_moves(SList_moves** poss_moves, int ln_moves, int lmax_moves);

// fonction remplaçant *poss_moves par les combinaisons de coups légales qu'il contient
void save_legal_possibles_moves(SList_moves** poss_moves, int ln_moves, int* legal_moves, int l_real_length);

void print_poss_moves(SList_moves** poss_moves, int ln_moves, int t_tab);

#endif