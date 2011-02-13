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
    int min_white_captures;
    int max_white_captures;
    int min_black_captures;
    int max_black_captures;
}pattern_t;

typedef struct raw_pattern_t{
    player spot_on_board[8];
}raw_pattern_t;

class Pattern_Match{
    private:
	vector< int> rule_matches;
	int priority_levels;
    public:
	Pattern_Match( int);
	~Pattern_Match();
	void record_match(int);
	int get_matches(int);
};

class Pente_AI{
    private:
	void initialize_rules( int = 0);
	void decipher_line( char*, int);
	raw_pattern_t look_at_board( Board&, Move_t, int, int);
	pattern_t convert_from_raw( Board&, raw_pattern_t);
	void match_to_rule( Board&, Pattern_Match&, pattern_t&);
	//TODO: pull the pattern from the board, mirror it, and match
	//the patterns against the rules list. Then decide among them
	//which is the best move. 
	vector< vector< pattern_t> > rules;
	vector< int> rules_in_line;
	  //rules_in_line is not an array index. It lists how
	  //many rules are there. 
	int priority_levels;
	  //priority_levels is an array index, so actual
	  //levels is n+1

    public:
	Pente_AI();
	void search_rectangle(Board&, int&, int&, int&, int&);
	  //left, right, top, bottom
	Move_t random_ai(Board&);
	Move_t narrow_random_ai(Board&);
	Move_t simple_rules_ai(Board&);
};

#endif
