#include "pente_ai.h"

Pente_AI::Pente_AI(){
}

void Pente_AI::search_rectangle(Board& the_board, int& left, int& right, int& top, int& bottom){
    left = 9, right = 9, top = 9, bottom = 9;
    Move_t temp_move;

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
}

Move_t Pente_AI::random_ai(Board& the_board){
    Move_t to_return;
    player the_player = the_board.get_player();

    do{
	to_return.x = rand() % 19;
	to_return.y = rand() % 19;
    }while( !the_board.move_is_legal( the_player, to_return));

    return to_return;
}

Move_t Pente_AI::narrow_random_ai(Board& the_board){
    Move_t to_return;
    player the_player = the_board.get_player();
    int left, right, top, bottom;

    search_rectangle( the_board, left, right, top, bottom);

    do{
	to_return.x = (rand() % (left - right)) + left;
	to_return.y = (rand() % (top - bottom)) + bottom;
    }while( !the_board.move_is_legal( the_player, to_return));

    return to_return;
}

Move_t Pente_AI::simple_rules_ai(Board& the_board){

    initialize_rules();

    //TODO: pick a move

    Move_t to_return;

    return to_return;
}

void Pente_AI::initialize_rules( int file_number){

    string file_to_open = "./Patterns/pattern_";
    const int size = 50; //I'll move this info elsewhere if needed. 
    char buffer[ size];

    priority_levels = -1; //This is to offset the initial addition of a row. 
//    rules.push_back( vector< pattern_t>());

    stringstream a;
    a << setfill('0') << setw(3) << file_number;
    file_to_open += a.str();
    file_to_open += ".txt";

    ifstream infile( file_to_open.c_str());

    infile.getline( buffer, size); //Need an input before an eof() test
    decipher_line( buffer, size);

    while( !infile.eof()){
	infile.getline( buffer, size);
	decipher_line( buffer, size);
    }

    infile.close();
}

void Pente_AI::decipher_line( char* buffer, int size){

    if( strncmp( buffer, "LEVEL", 5) == 0){
	rules.push_back( vector< pattern_t>());
	priority_levels++;
	rules_in_line.push_back( 0);
	rules_in_line[ priority_levels] = 0;
    }
    else if( strncmp( buffer, "\0", 1) == 0){
	//newline... do nothing
    }
    else if( strncmp( &buffer[4], "#", 1) == 0){
      //The lines look something like 
      // E??B#WWB?03
      // which is deciphered as follows: 
      // E is empty, ? is anything (including empty), # is the
      // prospective move, W and B stand for colors of stones
      // that must be there, the 0 is the min number of captures
      // that this rule applies to, and 3 is the max number of captures
      // the rule applies to. 

	pattern_t to_add;
	to_add.white = 0, to_add.black = 0, to_add.min_captures = 0, to_add.max_captures = 0;

	int j = 0;
	for( int i = 8 ; i >= 0 ; i--){
	    if( buffer[i] == '?' || buffer[i] == 'X'){
		to_add.white += (int)pow( (double)2, (double)j); //j to keep the power correct
	    }

	    if( buffer[i] == '?' || buffer[i] == 'B'){
		to_add.black += (int)pow( (double)2, (double)j);
	    }

	    //if( i == 4) i = 3; //This would bypass the # line, but I don't
	    //want to test it yet, and it won't be a problem anyway. 
	    if( i != 5) j++;
	}

	if( buffer[9] == '\0' || buffer[9] == '\n'){
	    to_add.min_captures = 0;
	    to_add.max_captures = 5;
	}
	else{
	    to_add.min_captures = buffer[9] - '0';
	    to_add.max_captures = buffer[10] - '0';
	}

	rules[ priority_levels].push_back( to_add);
	rules_in_line[ priority_levels]++;
    }
    else if( strncmp( &buffer[3], ",", 1) == 0){
      //These lines are in the format
      // 123,456,1:4
      // where 123 is the decimal representation of white,
      // 456 (outside of the pos range, but whatever) is of black, 
      // 1 is the min captures, 4 is the max captures the rule
      // applies to. 
	pattern_t to_add;
	to_add.white = 0, to_add.black = 0, to_add.min_captures = 0, to_add.max_captures = 0;

	to_add.white = atoi( buffer);
	to_add.black = atoi( &buffer[4]);

	if( buffer[8] == '\0'){
	    to_add.min_captures = 0;
	    to_add.max_captures = 5;
	}
	else{
	    to_add.min_captures = atoi( &buffer[8]);
	    to_add.max_captures = atoi( &buffer[10]);
	}

	rules[ priority_levels].push_back( to_add);
	rules_in_line[ priority_levels]++;
    }
}

raw_pattern_t look_at_board( Board the_board, Move_t, int x, int y){
  //Derection 0 is to the right, 1 is up right, 5 is down left, 7 is down right

    raw_pattern_t to_return;
    to_return.spot_on_board = "EEEEEEEE";

    if( x == 0 && y == 0){
	return to_return;
    }

//    to_return.spot_on_board[0] = the_board.get_spot( 

    return to_return;
}
