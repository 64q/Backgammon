#ifndef __GAME_AI_H__
#define __GAME_AI_H__

/**
 * Prise de décision de la part de l'IA
 * @param const game_t
 *	l'état du jeu courant
 * @param const player_t
 *	joueur actif à faire jouer
 * @param const moves_order_t*
 *	pointeur vers les mouvements à effectuer par l'IA
 * @return int
 *	code de retour défini par des constantes
 */
int make_decision(const game_t, const player_t, const moves_order_t*);

/**
 * Doubler la mise
 * @param const game_t
 *	l'état du jeu courant
 * @param const player_t
 *	joueur actif qui demande la mise
 * @return int
 *	true si on propose de doubler
 * 	false sinon
 */
bool double_stake(const game_t, const player_t);

/**
 * Accepter ou refuser la nouvelle mise
 * @param const game_t
 *	l'état du jeu courant
 * @param const player_t
 *	joueur non actif à qui on demande la mise
 * @return int
 *	true si on accepte la nouvelle mise
 * 	false sinon
 */
bool take_double(const game_t, const player_t);


/**
 * Initialiser l'IA pour un match
 * @param const unsigned int target_score
 *	score cible pour gagner un match
 * @return void
 */
void start_match(const unsigned int target_score);

/**
 * Initialiser l'IA pour une partie (d'un match)
 * @return void
 */
void start_game();

/**
 * Terminer une partie (d'un match)
 * @return void
 */
void end_game();

/**
 * Terminer un match
 * @return void
 */
void end_match();

#endif

