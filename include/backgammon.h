#ifndef __BKGM_H__
#define __BKGM_H__

/////////// Strutures  utiliser

// Reprsente un joueur
typedef enum
{
	EPlayer1,	// Joueur 1 -> le joueur courant pour la stratgie
	EPlayer2,	// Joueur 2
} EPlayer;

// Structure de reprsentation d'une zone du jeu
typedef struct
{
	EPlayer player;				// Joueur propritaire des pions
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

// Structure de reprsentation du jeu avec son plateau et les ds
typedef struct
{
	SZone zones[28];				// Tableau contenants les zones o les pions peuvent se trouver, indexs par des EPosition
	unsigned int die1,die2;			// Les ds de jeu
	unsigned int score, scoreP2;	// Les scores des joueurs
	unsigned int stake;				// Mise courante de la partie
} SGameState;

// Structure de reprsentation d'un mouvement
typedef struct
{
	EPosition src_point;
	EPosition dest_point;
} SMove;

// Dfinition des typedefs pour chargement dynamique
typedef void (*pfInitLibrary)(char[50]);
typedef void (*pfStartMatch)(const unsigned int);
typedef void (*pfStartGame)();
typedef void (*pfEndGame)();
typedef void (*pfEndMatch)();
typedef int (*pfDoubleStack)(const SGameState * const);
typedef int (*pfTakeDouble)(const SGameState * const);
typedef void (*pfMakeDecision)(const SGameState * const, SMove[4], unsigned int);

#endif

