#include <iostream>
#include <stdio.h>
#include "pente_definitions.h"
#include "board.h"
#include "pente_com.h"
#include "pente_ai.h"
using namespace std;

int main(){
    Pente_AI the_AI;
    Board the_board;
    player my_color;
    player op_color;
    Move_t temp;
    Pente_Com pente_com;

    srand( time(NULL));

    my_color = pente_com.get_color();
    op_color = -my_color;

    if( pente_com.get_color() == WHITE){
	temp = the_AI.narrow_random_ai( the_board);
	pente_com.play_move( temp);
	the_board.make_move( my_color, temp); //It is the individual AI's responsibility to keep track
					      //of moves/rules
    }

    while( !the_board.get_game_over()){

	temp = pente_com.get_opponents_move();
	the_board.make_move( op_color, temp);

	if( !the_board.get_game_over()){
	    temp = the_AI.narrow_random_ai( the_board);
	    pente_com.play_move( temp);
	    the_board.make_move( my_color, temp);
	}
    }

    return 1;
}
