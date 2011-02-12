#ifndef PENTE_AI_H
#define PENTE_AI_H

#include "pente_definitions.h"
#include "board.h"
#include <vector>
#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

typedef struct pattern_t{
    int white;
    int black;
    int min_captures;
    int max_captures;
}pattern_t;

typedef struct raw_pattern_t{
    char* spot_on_board;
}raw_pattern_t;

class Pente_AI{
    private:
	void initialize_rules( int = 0);
	void decipher_line( char*, int);
	raw_pattern_t look_at_board( Board, Move_t, int, int);
	//TODO: pull the pattern from the board, mirror it, and match
	//the patterns against the rules list. Then decide among them
	//which is the best move. 
	vector< vector< pattern_t> > rules;
	vector< int> rules_in_line;
	int priority_levels;

    public:
	Pente_AI();
	void search_rectangle(Board&, int&, int&, int&, int&);
	Move_t random_ai(Board&);
	Move_t narrow_random_ai(Board&);
	Move_t simple_rules_ai(Board&);
};

#endif
