#include <stdio.h>
#include <stdlib.h>

#include "../include/backgammon.h"
#include "../include/moves.h"


int calc_moves(SGameState* gameState, SList_moves** possibles_moves, int* lp_moves, int move_order, int move_order_limit)
{
	if (move_order_limit == 0) // dans ce cas la fonction vient d'être appelé par MakeDecision
	{
		if (gameState->die1 == gameState->die2) 
		{
			move_order_limit = 4; // 4 coups si on fait un double
		}
		else
		{
			move_order_limit = 2; // sinon 2
		}
	}
	
	int ln_moves = (*lp_moves);
	int cont = 1; 
	int i;
	
	if(move_order == 0)
	{		
		ln_moves = init_combi_moves(gameState, possibles_moves); // calcule des coups primaires jouables
		
		for (i = 0; i < ln_moves; i++)
		{
			if (gameState->die1 == gameState->die2) // cas d'un double
			{
				((*possibles_moves) + i)->pmoves = gameState->die1; // pmoves vaut le dé 1
			}
			else
			{
				int src_point = (((*possibles_moves) + i)->head.src_point);
				int dest_point = (((*possibles_moves) + i)->head.dest_point);
				
				if (src_point == EPos_BarP1)
				{
					src_point = EPos_24 +1;
				}
				if (dest_point == EPos_OutP1)
				{
					dest_point = EPos_1 - 1;
				}
			
				if ((src_point - dest_point) == gameState->die1) // si le dé 1 a été joué
				{
					((*possibles_moves) + i)->pmoves = gameState->die2; // il reste le 2
				}
				else
				{
					if ((src_point - dest_point) == gameState->die2)
					{
						((*possibles_moves) + i)->pmoves = gameState->die1;
					}
					else
					{
						if ((src_point - dest_point) < gameState->die1)
						{
							((*possibles_moves) + i)->pmoves = gameState->die2;
						}
						else
						{
							((*possibles_moves) + i)->pmoves = gameState->die1;
						}
					}
				}
			}
		}
	}
	else
	{
		{		
			cont = 0;
			
			for (i = 0; i < (*lp_moves); i++) // pour chaque combinaison de coup
			{
				cont += complete_list((*gameState), ((*possibles_moves) + i), move_order); // on la complète suivi l'avancée dans l'ordre de mouvement
			}
			
			if (cont > 1)
			{
				cont = 1;
			}
		}
	}
	
	if ((move_order != 0) || (ln_moves != (*lp_moves)))
	{
		if ((move_order+1 < move_order_limit) && (cont == 1))
		{
			if (move_order == 0)
			{
				*lp_moves = ln_moves;
			}
			return calc_moves(gameState, possibles_moves, lp_moves, move_order+1, move_order_limit);
		}
	}
	
	if ((move_order+cont) < move_order_limit)
	{
		return move_order;
	}
	
	else
	{
		return (move_order + 1);
	}
}

int init_combi_moves(SGameState* gameState, SList_moves** poss_moves)
{
	SMove** new_moves; // pointeur sur tableau de coups
	new_moves = (SMove**) malloc(sizeof(SMove*));
	*new_moves = NULL;
	
	int i;
	int ln_moves = calc_primary_moves(gameState, new_moves);
	
	for (i = 0; i < ln_moves; i++)
	{				
		add_list_moves(poss_moves,i);
		
		((*poss_moves) + i)->head = (*((*new_moves)+i));
		((*poss_moves) + i)->nexts = NULL;
		((*poss_moves) + i)->l_nexts = 0;
	}
	
	free(*new_moves);
	*new_moves = NULL;
	
	free(new_moves);
	new_moves = NULL;
	
	return ln_moves;
}

int calc_primary_moves(SGameState* gameState, SMove** new_moves)
{
	int ln_moves = 0;
	
	int die1 = gameState->die1;
	int die2 = gameState->die2;
	SZone* zones = gameState->zones;
	
	if (zones[EPos_BarP1].nb_checkers > 0)
	{
		ln_moves = from_board(new_moves, ln_moves, zones, 24, die1, 0, 1);
		
		if (die1 != die2)
		{
			ln_moves = from_board(new_moves, ln_moves, zones, 24, die2, 0, 1);
		}
	}
	
	else
	{
		int i;
		int out_ok = 1;
		int first_found = 1;
	
		for (i = EPos_24; i >= EPos_1; i--)
		{
			if ((zones[i].player == EPlayer1) && (zones[i].nb_checkers > 0))
			{				
				if (i > 6)
				{
					out_ok = 0;
				}
				
				ln_moves = from_board(new_moves, ln_moves, zones, i, die1, out_ok, first_found);
				if (die1 != die2)
				{
					ln_moves = from_board(new_moves, ln_moves, zones, i, die2, out_ok, first_found);
				}
				
				first_found = 0;
			}
		}
	}
	
	return ln_moves;
}

int from_board(SMove** new_moves, int ln_moves, SZone zones[28], int src_point, int die, int out_ok, int first_found)
{
	int dest_point = src_point - die;

	if ((dest_point >= EPos_1) || ((out_ok == 1) && ((dest_point == (EPos_1 - 1)) || ((first_found == 1) && (dest_point < EPos_1)))))
	{		
		if (dest_point <= -1)
		{
			dest_point = EPos_OutP1;
		}
		
		if ((zones[dest_point].player == EPlayer1) || (zones[dest_point].nb_checkers <= 1))
		{
			SMove move;
			
			if (src_point == 24)
			{
				src_point = EPos_BarP1;
			}
			
			move.src_point = src_point;
			move.dest_point = dest_point;
			
			ln_moves = add_move(new_moves,ln_moves,move);
		}
	}
	
	return ln_moves;
}

int complete_list(SGameState gameState, SList_moves* plist, int move_order)
{
	int i;

	if (move_order == 0)
	{
		SList_moves** p_nexts = (SList_moves**) malloc(sizeof(SList_moves*));
		*p_nexts = NULL;
		
		plist->l_nexts = init_combi_moves(&gameState, p_nexts);
		
		plist->nexts = *p_nexts;
		
		for (i = 0; i < plist->l_nexts; i++)
		{				
			((plist->nexts) + i)->pmoves = plist->pmoves;
		}
		
		free(p_nexts);
		p_nexts = NULL;
		
		if (plist->l_nexts == 0)
		{
			return 0;
		}	
		else
		{
			return 1;
		}
	}
	
	else
	{
		SGameState temp_gameState = update_game_state(gameState, *plist);
	
		if (move_order == 1)
		{		
			return complete_list(temp_gameState, plist, (move_order - 1));
		}
		
		else
		{
			int cont = 0;
			
			for (i = 0; i < (plist->l_nexts); i++)
			{
				cont += complete_list(temp_gameState, ((plist->nexts) + i), (move_order - 1));
			}
			
			if (cont != 0)
			{
				return 1;
			}
		}
	}
	
	return 0;
}

SGameState update_game_state(SGameState gameState, SList_moves list)
{
	SMove move = list.head;
	gameState.zones[move.src_point].nb_checkers -= 1;
	
	if (gameState.zones[move.dest_point].player == EPlayer2)
	{
		if (gameState.zones[move.dest_point].nb_checkers == 1)
		{
			gameState.zones[EPos_BarP2].nb_checkers += 1;
		}
		gameState.zones[move.dest_point].player = EPlayer1;
		gameState.zones[move.dest_point].nb_checkers = 1;
	}
	else
	{
		gameState.zones[move.dest_point].player = EPlayer1;
		gameState.zones[move.dest_point].nb_checkers += 1;
	}
	
	if ((gameState.die1 != list.pmoves) || (gameState.die2 != list.pmoves))
	{
		gameState.die1 = list.pmoves;
		gameState.die2 = list.pmoves;
	}
	
	return gameState;
}

int add_move(SMove** moves, int lmoves, SMove move)
{
	if (lmoves == 0)
	{
		*moves = (SMove*) malloc(sizeof(SMove));
	}
	
	else
	{
		*moves = (SMove*) realloc(*moves, (sizeof(SMove) * (lmoves + 1)));
	}

	*((*moves) + lmoves) = move;

	return (lmoves + 1);
}

int add_list_moves(SList_moves** moves, int lmoves)
{
	if (lmoves == 0)
	{
		*moves = (SList_moves*) malloc(sizeof(SList_moves));
	}
	
	else
	{
		*moves = (SList_moves*) realloc(*moves, (sizeof(SList_moves) * (lmoves + 1)));
	}
	
	return (lmoves + 1);
}

int clean_possibles_moves(SList_moves** poss_moves, int ln_moves, int lmax_moves)
{
	int i;
	int legal_moves[ln_moves-1];
	int l_real_length = 0;

	for (i = 0; i < ln_moves; i++)
	{
		SList_moves* pfils = ((*poss_moves)+ i);
	
		if (pfils->l_nexts != 0)
		{
			SList_moves** pn_moves;
			pn_moves = &(pfils->nexts);
			
			pfils->l_nexts = clean_possibles_moves(pn_moves, pfils->l_nexts, lmax_moves - 1);
			
			pfils->nexts = *pn_moves;
		}
		
		if (pfils->l_nexts != 0 || lmax_moves == 1)
		{
			legal_moves[l_real_length] = i;
			l_real_length += 1;
		}
		else
		{
			free(pfils->nexts);
			pfils->nexts = NULL;
		}
	}
	
	if (l_real_length < ln_moves)
	{
		save_legal_possibles_moves(poss_moves, ln_moves, legal_moves, l_real_length);	
	}
	
	return (l_real_length);
}

void save_legal_possibles_moves(SList_moves** poss_moves, int ln_moves, int* legal_moves, int l_real_length)
{
	int i;
	
	SList_moves new_poss_moves[l_real_length];
	
	for (i = 0; i < l_real_length; i++)
	{
		new_poss_moves[i] = (*((*poss_moves) + legal_moves[i]));
	}
	
	*poss_moves = (SList_moves*) realloc(*poss_moves, sizeof(SList_moves) * l_real_length);
	
	poss_moves = (SList_moves**) (&new_poss_moves);
}

void free_SList_moves(SList_moves** m_list, int ln_moves, int max)
{
	int i;
	
	if (max > 0)
	{
		for (i = 0; i < ln_moves; i++)
		{
			free_SList_moves(&((*m_list)[i].nexts), (*m_list)[i].l_nexts, max - 1);
		}
	}
	free(*m_list);
}

void print_poss_moves(SList_moves** poss_moves, int ln_moves, int t_tab)
{
	int i,j;
	
	for (i = 0; i < ln_moves; i++)
	{
		for (j = 0; j < t_tab; j++)
		{
			printf("     ");
		}
	
		SList_moves* plist = ((*poss_moves)+i);
		printf("%i/%i\n", plist->head.src_point+1, plist->head.dest_point+1);
		
		if (plist->l_nexts != 0)
		{
			SList_moves** pm = (SList_moves**) malloc(sizeof(SList_moves*));
			*pm = plist->nexts;
	
			print_poss_moves(pm, plist->l_nexts, t_tab + 1);
	
			
			free(pm);
		}
	}
}