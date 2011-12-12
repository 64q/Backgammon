#ifndef __GAME_ENGINE_H__
#define __GAME_ENGINE_H__

// Représente un joueur
enum player {
	P1, // Joueur 1
	P2, // Joueur 2
	NP  // Aucun joueur
};
typedef enum player player_t;

// Représente un triangle dans le jeu
struct point {
	player_t player; 	 // Joueur courant
	unsigned int nb_checkers; // Nombre de pion du joueur sur le triangle
};
typedef struct point point_t;

// Constantes des zones pour le tableau points
const int P1_OUT = 0;
const int P2_OUT = 25;
const int P1_BAR = 26;
const int P2_BAR = 27;

// Cette structure représente le jeu avec son plateau et les dés
struct game {
	/* Tableau contenants les zones où les pions peuvent se trouver
	 * Le joueur 1 se déplace dans le sens décroissant
	 * Le joueur 2 se déplace dans le sens croissant
	 * Indices des flèches : 1 à 24
	 * Indices des zones de sortie et des pions prisonniers
	 * définies par les constantes ci-dessus
	*/
	point_t points[28];

	// Les dés de jeu
	unsigned int die1;
	unsigned int die2;

	// Les scores des joueurs
	unsigned int p1_score;
	unsigned int p2_score;

	// Score à atteindre pour gagner le match
	unsigned int target_score;

	// Mise de la partie
	unsigned int stake;

	// Propriétaire du videau
	player_t cube_owner;
};
typedef struct game game_t;

// Structure de représentation d'un mouvement
struct move {
	unsigned int src_point;
	unsigned int dest_point;
};
typedef struct move move_t;

struct moves_order {
	move_t moves[4];
	unsigned int nb_moves;
};
typedef struct moves_order moves_order_t;

/* Codes de retour pour la fonction de décision
 * AI_SUCCESS : l'IA n'a rencontré aucun problème
 * AI_PARAM_ERROR : Erreur de paramètres
 * AI_BOARD_ERROR : Erreur de pions mal placés sur le plateau
 */
const int AI_SUCCESS = 0;
const int AI_PARAM_ERROR = 1;
const int AI_BOARD_ERROR = 2;

#endif

