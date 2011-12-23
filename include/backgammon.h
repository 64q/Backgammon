#ifndef __BKGM_H__
#define __BKGM_H__

/////////// Strutures � utiliser

// Repr�sente un joueur
typedef enum
{
	EPlayer1,	// Joueur 1 -> le joueur courant pour la strat�gie
	EPlayer2,	// Joueur 2
} EPlayer;

// Structure de repr�sentation d'une zone du jeu
typedef struct
{
	EPlayer player;				// Joueur propri�taire des pions
	unsigned int nb_checkers;	// Nombre de pion du joueur sur la zone
} SZone;

// Enumeration des zones pour le tableau points
typedef enum
{
	EPos_1 = 0,
	EPos_2,
	EPos_3,
	EPos_4,
	EPos_5,
	EPos_6,
	EPos_7,
	EPos_8,
	EPos_9,
	EPos_10,
	EPos_11,
	EPos_12,
	EPos_13,
	EPos_14,
	EPos_15,
	EPos_16,
	EPos_17,
	EPos_18,
	EPos_19,
	EPos_20,
	EPos_21,
	EPos_22,
	EPos_23,
	EPos_24,
	EPos_OutP1,
	EPos_BarP1,
	EPos_OutP2,
	EPos_BarP2
} EPosition;

// Structure de repr�sentation du jeu avec son plateau et les d�s
typedef struct
{
	SZone zones[28];				// Tableau contenants les zones o� les pions peuvent se trouver, index�s par des EPosition
	unsigned int die1,die2;			// Les d�s de jeu
	unsigned int scoreP1, scoreP2;	// Les scores des joueurs
	unsigned int stake;				// Mise courante de la partie
} SGameState;

// Structure de repr�sentation d'un mouvement
typedef struct
{
	EPosition src_point;
	EPosition dest_point;
} SMove;

// D�finition des typedefs pour chargement dynamique
typedef void (*pfInitLibrary)(char[50]);
typedef void (*pfStartMatch)(const unsigned int);
typedef void (*pfStartGame)();
typedef void (*pfEndGame)();
typedef void (*pfEndMatch)();
typedef int (*pfDoubleStack)(const SGameState * const);
typedef int (*pfTakeDouble)(const SGameState * const);
typedef void (*pfMakeDecision)(const SGameState * const, SMove[4], unsigned int);

#endif

