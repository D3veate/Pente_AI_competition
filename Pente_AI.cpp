#include "pente_ai.h"

Pente_AI::Pente_AI(){
    rules_initialized = false;
}

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

    cout << "L: " << left << " R: " << right << " T: " << top << " B: " << bottom << endl;
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

Move Pente_AI::narrow_random_ai(Board& the_board){
    Move to_return;
    player the_player = the_board.get_player();
    int left, right, top, bottom;

    search_rectangle( the_board, left, right, top, bottom);

    do{
	to_return.x = (rand() % (left - right)) + left;
	to_return.y = (rand() % (top - bottom)) + bottom;
    }while( !the_board.move_is_legal( the_player, to_return));

    return to_return;
}

Move Pente_AI::simple_rules_ai(Board& the_board){
    if( !rules_initialized){
	initialize_rules();
    }

    Move to_return;

    return to_return;
}

void Pente_AI::initialize_rules( int file_number){

    string file_to_open = "./Patterns/pattern_";

    stringstream a;
    a << setfill('0') << setw(3) << file_number;
    file_to_open += a.str();
    file_to_open += ".txt";

    cout << "File: " << file_to_open << endl;

//    infile = ifstream("
}
