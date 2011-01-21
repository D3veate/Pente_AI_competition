#include "pente_ai.h"

void Pente_AI::search_rectangle(Board& the_board, int& left, int& right, int& top, int& bottom){
    left = 9, right = 9, top = 9, bottom = 9;
    Move temp_move;

    for( int i = 0 ; i < the_board.get_moves_made() ; i++){
	temp_move = the_board.get_move( i);

	left = temp_move.x < left ? temp_move.x : left;
	right = temp_move.x > right ? temp_move.x : right;
	top = temp_move.y > top ? temp_move.y : top;
	bottom = temp_move.y < bottom ? temp_move.y : bottom;
    }

    left = left < 4 ? 0 : left - 4; //expand the box
    right = right > 14 ? 18 : right + 4;
    bottom = bottom < 4 ? 0 : bottom - 4;
    top = top > 14 ? 18 : top + 4;

    cout << left << right << bottom << top << endl;
}

Move Pente_AI::random_ai(Board& the_board){
    Move to_return;
    player the_player = the_board.get_player();

    do{
	to_return.x = rand() % 19;
	to_return.y = rand() % 19;
    }while( !the_board.move_is_legal( the_player, to_return));

    return to_return;
}
