#ifndef __MOVES_H__
#define __MOVES_H__

#include "../include/backgammon.h"

// structure d'arbre représentant des combinaisons de coups
typedef struct SList_moves
{
	SMove head; // mouvement de tête
	int l_nexts; // taille de nexts
	struct SList_moves* nexts; // tableau de SList_moves correspondant aux coups suivants
	
	int pmoves; // valeur du dé encore jouable
} SList_moves;

/*
 * Calculer les différentes combinaisons de coups possibles
 * @param SGameState* gameState
 *  état du jeu que l'on va évaluer
 * @param SList_moves** possibles_moves
 *  structure d'arbre où seront stockées les différentes combinaisons
 * @param int* lp_moves
 *  nombre de SList_moves dans (*possibles_moves)
 * @param int move_order
 *  profondeur des mouvements à ajouter aux combinaisons de coups
 *  doit être à 0 à l'initialisation de la fonction
 * @param int move_order_limit
 *  nombre maximal de coups dans une combinaison de coups
 *  doit être à 0 à l'initialisation de la fonction
 * @return int
 *  retourne la taille de la plus grande combinaison de coups
 */
int calc_moves(SGameState* gameState, SList_moves** possibles_moves, int* lp_moves, int move_order, int move_order_limit);

/*
 * Initialiser un tableau de SList_moves à partir d'un gameState
 * @param SGameState* gameState
 *  état du jeu que l'on va évaluer
 * @param SList_moves** poss_moves
 *  structure d'arbre où seront stockées les différentes combinaisons
 * @return int
 *  nombre de SList_moves contenues par (*poss_moves)
 */
int init_combi_moves(SGameState* gameState, SList_moves** poss_moves);

/*
 * Calculer les différents mouvements (primaires) possibles
 * @param SGameState* gameState
 *  état du jeu que l'on va évaluer
 * @param SMove** new_moves
 *  tableau qui va contenir les différents mouvements possibles
 * @return int
 *  taille de (*new_moves)
 */
int calc_primary_moves(SGameState* gameState, SMove** new_moves);

/*
 * Valider un mouvement sur le tableau et l'ajouter s'il a été validé
 * @param SMove** new_moves
 *  tableau auquel on ajoutera le mouvement validé
 * @param int ln_moves
 *  taille de (*new_moves) à l'initialisation de la fonction
 * @param SZone zones[28]
 *  zones détaillant le plateau de jeu
 * @param int src_point
 *  point de départ du mouvement
 * @param int die
 *  valeur du dé que l'on veut utiliser pour le mouvement
 * @param out_ok
 *  vaut 1 si on peut sortir nos pions en EPos_OutP1, 0 sinon
 * @param first_found
 *  vaut 1 si src_point équivaut à la position du pion le plus éloigné de la sortie (le premier trouvé) 0 sinon
 * @return int
 *  nouvelle valeur de ln_moves
 */
int from_board(SMove** new_moves, int ln_moves, SZone zones[28], int src_point, int die, int out_ok, int first_found);

/*
 * Compléter un tableau de liste comportant des mouvements "primaires"
 * @param SGameState gameState
 *  état du plateau sur lequel on va se baser
 * @param SList_moves* plist
 *  SList_moves à compléter
 * @param int move_order
 *  profondeur des coups à rajouter
 * @return int
 *  retourne 1 si des coups ont été rajoutés, 0 sinon
 */
int complete_list(SGameState gameState, SList_moves* plist, int move_order);

/*
 * Modifier un SGameState
 * @param SGameState gameState
 *  gamestate à modifier
 * @param SList_moves list
 *  structure contenant les informations pour modifier le gameState (seulement le mouvement head est appliqué)
 * @return SGameState
 *  SGameState modifié
 */
SGameState update_game_state(SGameState gameState, SList_moves list);

/*
 * Ajouter un mouvement à un tableau de mouvements
 * @param SMove** moves
 *  tableau de mouvements auquel on ajoute le mouvement
 * @param int lmoves
 *  taille avant ajout du tableau
 * @param SMove move
 *  mouvement à ajouter
 */
int add_move(SMove** moves, int lmoves, SMove move);

/*
 * Ajouter une SList_moves vide à un tableau de SList_moves
 * @param SList_moves** moves
 *  le tableau qui va la contenir
 * @param int lmoves
 *  sa taille avant l'ajout
 * @return int
 *  la nouvelle taille après l'ajout
 */
int add_list_moves(SList_moves** moves, int lmoves);

/*
 * Supprimer les combinaisons interdites
 * @param SList_moves** poss_moves
 *  arbre contenant des combinaisons de coups
 * @param int ln_moves
 *  taille de l'arbre
 * @param int lmax_moves
 *  profondeur maximale à respecter
 * @return int
 *  nouvelle taille de poss_moves
 */
int clean_possibles_moves(SList_moves** poss_moves, int ln_moves, int lmax_moves);

/*
 * Conserver uniquement les combinaisons autorisées
 * @param SList_moves** poss_moves
 *  arbre des combinaisons
 * @param int ln_moves
 *  taille de l'arbre
 * @param int* legal_moves
 *  tableau des indices des combinaisons autorisées
 * @param int l_real_length
 *  taille de ce tableau
 */
void save_legal_possibles_moves(SList_moves** poss_moves, int ln_moves, int* legal_moves, int l_real_length);

/*
 * Libérer un tableau de SList_moves récursivement
 * @param SList_moves** m_list
 *  tableau à libérer
 * @param int ln_moves
 *  taille du tableau
 * @param int max
 *  pronfondeur max
 */
void free_SList_moves(SList_moves** m_list, int ln_moves, int max);

/*
 * Afficher un arbre
 * @param SList_moves** poss_moves
 *  arbre à afficher
 * @param int ln_moves
 *  taille du tableau initial
 * @param t_tab
 *  nombre de tabulations à afficher devant chaque ligne
 *  généralement 0 à l'initialisation
 */
void print_poss_moves(SList_moves** poss_moves, int ln_moves, int t_tab);

#endif
