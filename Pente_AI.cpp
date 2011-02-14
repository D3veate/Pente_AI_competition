#include "pente_ai.h"

Pattern_Match::Pattern_Match( int priority_levels_incoming)
	      :priority_levels( priority_levels_incoming){
    for( int i = 0 ; i < priority_levels ; i++){
	rule_matches.push_back(0);
    }
}

Pattern_Match::~Pattern_Match(){
    rule_matches.clear();
}

void Pattern_Match::record_match( int priority_level){
    rule_matches[ priority_level]++;
}

int Pattern_Match::get_matches( int priority_level){
    return rule_matches[ priority_level];
}

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

    int left, right, top, bottom;
    search_rectangle( the_board, left, right, top, bottom);

    vector< vector<Pattern_Match> > records;
    for( int i = 0 ; i < 19 ; i++){

	records.push_back( vector<Pattern_Match>() );
	for( int j = 0 ; j < 19 ; j++){
	    records[i].push_back( Pattern_Match( priority_levels));
	}
    } //TODO: use the clear() on these.

    /*
    Pattern_Match** records = new Pattern_Match*[19]; //Several unused ones here. 
    for( int i = 0 ; i < 19 ; i++){
	records[i] = new Pattern_Match[19]; //the new[] operator doesn't work either. Here, the progam
					    //is trying to create the objects but doesn't have a constructor
					    //match.
	for( int j = 0 ; j < 19 ; j++){
	    records[i][j] = new Pattern_Match( priority_levels);
	}
    }
    //need a 2d-array of matches...
    */

    /*
     * This code compiles: 

    Pattern_Match** records;
    for( int i = 0 ; i < 19 ; i++){
	for( int j = 0 ; j < 19 ; j++){
	    records[i][j] = Pattern_Match( priority_levels); //warning: records may be uninitialized...
	}
    }
    */


    for( int i = left ; i <= right ; i++){
	for( int j = bottom ; j <= top ; j++){ //This part of the loop chooses the spot

	    Move_t possible_spot;
	    possible_spot.x = i;
	    possible_spot.y = j;

	    if( the_board.get_spot( possible_spot) != EMPTY)
		continue;

	    for( int p = -1 ; p <= 1 ; p++){
		for( int q = -1 ; q <= 1 ; q++){ //these choose direction

		    if( p == 0 && q == 0) 
			continue;

		    raw_pattern_t temp_raw = look_at_board( the_board, possible_spot, p, q);
		    pattern_t temp_pattern = convert_from_raw( the_board, temp_raw);
		    match_to_rule( the_board, records[i][j], temp_pattern);
		}
	    }
	}
    }

    //pick best match

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
	to_add.white = 0, to_add.black = 0;
	to_add.min_white_captures = 0, to_add.max_white_captures = 0;
	to_add.min_black_captures = 0, to_add.max_black_captures = 0;

	int j = 0;
	for( int i = 8 ; i >= 0 ; i--){
	    if( buffer[i] == '?' || buffer[i] == 'W'){
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
	    to_add.min_white_captures = 0;
	    to_add.max_white_captures = 5;
	    to_add.min_black_captures = 0;
	    to_add.max_black_captures = 5;
	}
	else{
	    to_add.min_white_captures = buffer[9] - '0';
	    to_add.max_white_captures = buffer[10] - '0';
	    to_add.min_black_captures = buffer[11] - '0';
	    to_add.max_black_captures = buffer[12] - '0';
	}

	rules[ priority_levels].push_back( to_add);
	rules_in_line[ priority_levels]++;
    }
    else if( strncmp( &buffer[3], ",", 1) == 0){
      //These lines are in the format
      // 123,456,1423
      // where 123 is the decimal representation of white,
      // 456 (outside of the pos range, but whatever) is of black, 
      // 1 is the min captures, 4 is the max captures (white) the rule
      // applies to. while 2 and 3 are the ranges of black captures
	pattern_t to_add;
	to_add.white = 0, to_add.black = 0;
	to_add.min_white_captures = 0, to_add.max_white_captures = 0;
	to_add.min_black_captures = 0, to_add.max_black_captures = 0;

	to_add.white = atoi( buffer);
	to_add.black = atoi( &buffer[4]);

	if( buffer[8] == '\0'){
	    to_add.min_white_captures = 0;
	    to_add.max_white_captures = 5;
	    to_add.min_black_captures = 0;
	    to_add.max_black_captures = 5;
	}
	else{
	    to_add.min_white_captures = buffer[8] - '0';
	    to_add.max_white_captures = buffer[9] - '0';
	    to_add.min_black_captures = buffer[10] - '0';
	    to_add.max_black_captures = buffer[11] - '0';
	}

	rules[ priority_levels].push_back( to_add);
	rules_in_line[ priority_levels]++;
    }
}

raw_pattern_t Pente_AI::look_at_board( Board& the_board, Move_t potential_move, int x, int y){
  //Derection 0 is to the right, 1 is up right, 5 is down left, 7 is down right

    raw_pattern_t to_return;

    for( int i = 0 ; i < 8 ; i++){
	to_return.spot_on_board[i] = EMPTY;
    }

    if( x == 0 && y == 0){
	return to_return;
    }

    Move_t spot_looking_at;
    for( int i = 4 ; i >= 1 ; i--){
	spot_looking_at.x = potential_move.x - (x * i); //affect the opposite direction 
							//described by x and y
	spot_looking_at.y = potential_move.y - (y * i);

	if( spot_looking_at.x < 0 || spot_looking_at.x > 18
	 || spot_looking_at.y < 0 || spot_looking_at.y > 18){
	    to_return.spot_on_board[ 4 - i] = EMPTY; //Off the board. 
	}
	else{
	    to_return.spot_on_board[ 4 - i] = the_board.get_spot( spot_looking_at);
	}
    }

    for( int i = 1 ; i <= 4 ; i++){
	spot_looking_at.x = potential_move.x + (x * i); //now going in the direction declared
	spot_looking_at.y = potential_move.y + (y * i);

	if( spot_looking_at.x < 0 || spot_looking_at.x > 18
	 || spot_looking_at.y < 0 || spot_looking_at.y > 18){
	    to_return.spot_on_board[ 3 + i] = EMPTY; //Off the board. The 3 is to start this loop
						   //on the fourth index
	}
	else{
	    to_return.spot_on_board[ 3 + i] = the_board.get_spot( spot_looking_at);
	}
    }

    return to_return;
}

pattern_t Pente_AI::convert_from_raw( Board& the_board, raw_pattern_t raw){
    pattern_t to_return;

    to_return.min_white_captures = the_board.get_white_captures();
    to_return.max_white_captures = to_return.min_white_captures;
    to_return.min_black_captures = the_board.get_black_captures();
    to_return.max_black_captures = to_return.min_black_captures;

    to_return.white = 0;
    to_return.black = 0;

    int current_pow;
    for( int i = 0 ; i < 8 ; i++){
	current_pow = (int)pow( (double)2, (double)i);

	if( raw.spot_on_board[i] == WHITE){
	    to_return.white += current_pow;
	}
	else if( raw.spot_on_board[i] == BLACK){
	    to_return.black += current_pow;
	}
    }

    return to_return;
}

void Pente_AI::match_to_rule( Board& the_board, Pattern_Match& the_matches, pattern_t& the_pattern){
    for( int i = 0 ; i <= priority_levels ; i++){ //note, priority_levels and rules in line weren't constructed
						  //in the same way
	for( int j = 0 ; j < rules_in_line[ priority_levels] ; j++){
	    if( (((the_pattern.white & rules[i][j].white) == the_pattern.white) && ((the_pattern.black & rules[i][j].black) == the_pattern.black))
	     && (the_pattern.min_white_captures >= rules[i][j].min_white_captures && the_pattern.max_white_captures <= rules[i][j].max_white_captures)
	     && (the_pattern.min_black_captures >= rules[i][j].min_black_captures && the_pattern.max_black_captures <= rules[i][j].max_black_captures)){

		the_matches.record_match(i);

		return; //Early return because I don't want a particular pattern patch on a high priority to trigger all 
			//lower priorities and have some extra influence over the outcome. 
	     }
	}
    }
}
