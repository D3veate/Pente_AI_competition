#ifndef BOARD_HEADER_H
#define BOARD_HEADER_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include "pente_definitions.h"
using namespace std;

class Board{
    private:
	int** spot; //19 x 19
	vector<Move> moves;

	int moves_made;
	int turn;
	bool game_over;
	int winner;
	int white_captures;
	int black_captures;

	bool is_game_over_helper();
	void clear_board_helper();
	void perform_captures_helper(player, Move);

    public:
	Board();
	Board(const Board&);
	~Board();

	Board operator=(const Board&);
	Board operator--();
	Board operator--(int);

	void display( bool = false);

	bool move_is_legal(player, Move);
	bool make_move(player, Move); //returns true if move is made, false if move is illegal

	player get_spot(Move);
	int get_moves_made();
	int get_turn();
	bool get_game_over();
	int get_winner();
	int get_white_captures();
	int get_black_captures();

	player get_player();
	Move get_move(int);
};

#endif
