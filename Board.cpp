#include "board.h"

Board::Board()
      :moves_made(0), turn(WHITE), game_over(false), winner(EMPTY), white_captures(0), black_captures(0){
    spot = new int*[19];

    for( int i = 0 ; i < 19 ; i++){

	spot[i] = new int[19];

	for( int j = 0 ; j < 19 ; j++){
	    spot[i][j] = EMPTY;
	}
    }
}

Board::Board(const Board& copy_from)
      :moves_made(copy_from.moves_made), turn(copy_from.turn), game_over(copy_from.game_over), winner(copy_from.winner),
      white_captures(copy_from.white_captures), black_captures(copy_from.black_captures){

    this -> spot = new int*[19];

    for( int i = 0 ; i < 19 ; i++){

	this -> spot[i] = new int[19];

	for( int j = 0 ; j < 19 ; j++){
	    spot[i][j] = copy_from.spot[i][j];
	}
    }

    for(int i = 0 ; i < copy_from.moves_made ; i++){
	moves.push_back( copy_from.moves[i]);
    }
}

Board::~Board(){
    for( int i = 0 ; i < 19 ; i++){
	delete[] spot[i];
    }

    delete[] spot;

    moves.clear();
}

Board Board::operator=(const Board& copy_from){ //incomplete
    for( int i = 0 ; i < 19 ; i++){
	for( int j = 0 ; j < 19 ; j++){
	    spot[i][j] = copy_from.spot[i][j];
	}
    }

    moves_made = copy_from.moves_made;

    moves.resize( moves_made);
    for( int i = 0 ; i < moves_made ; i++){
	moves[i] = copy_from.moves[i];
    }

    turn = copy_from.turn;
    game_over = copy_from.game_over;

    return *this;
}

Board Board::operator--(){
//    *this = Board();

    if( moves_made != 0){
	int temp = moves_made - 1;
	clear_board_helper();

	for( int i = 0 ; i < temp ; i++ ){
	    if( i % 2 == 0 && make_move( WHITE, moves[i])){} //function call done thus as an error message convenience
	    else if( make_move( BLACK, moves[i])){}
	    else{
		cout << "error in  Board Board::operator--() " << endl;
	    }
	}

	moves.pop_back();
//	moves_made--; //moves_made is incremented during each make_move() call
    }

    return *this;
}

Board Board::operator--(int dummy){
    return --(*this);
}

/* The display function is increadibly ugly, but
 * I couldn't figure out how to create a variable
 * that can reference cout or cerr depending on
 * the value of errstream
 */
void Board::display(bool errstream){
    Move_t temp;

    if( errstream == false){
	cout << "    ";

	for( int i = 0 ; i < 19 ; i++){
	    cout << setw(3) << i << " ";
	}

	cout << endl;

	for( int j = 18 ; j >= 0 ; j--){
	    cout << setw(2) << j << "  ";

	    for( int i = 0 ; i < 19 ; i++){
		cout << setw(3);

		temp.x = i;
		temp.y = j;
		if( get_spot( temp) == BLACK){
		    cout << "O";
		}
		else if( get_spot( temp) == WHITE){
		    cout << "X";
		}
		else if( get_moves_made() == 0){ //initial... shows a * in the center
		    if( i == 9 && j == 9){
			cout << "*";
		    }
		    else{
			cout << ".";
		    }
		}
		else if( get_moves_made() == 2){ //on white's second move shows a * box indicating legal moves
		    if( (i == 6 && ( j >= 6 && j <= 12))
		     || (i == 12 && ( j >= 6 && j <= 12))
		     || ((i >= 6 && i <= 12) && j == 6)
		     || ((i >= 6 && i <= 12) && j == 12 )){

			cout << "*";
		    }
		    else{
			cout << ".";
		    }
		}
		else{
		    cout << ".";
		}
		cout << " ";
	    }

	    cout << endl << endl;
	}

	cout << "Moves made: " << get_moves_made();
	cout << " White's captures: " << get_white_captures();
	cout << " Black's captures: " << get_black_captures();

	if( get_game_over() == true){
	    cout << "\t\tThe game is over. The winner is ";

	    if( get_winner() == WHITE){
		cout << "white!" << endl;
	    }
	    else{
		cout << "black!" << endl;
	    }
	}
	else{
	    if( turn == WHITE){
		cout << "\t\tWhite's turn. ";
	    }
	    else{
		cout << "\t\tBlack's turn. ";
	    }
	}


	cout << endl;
    }
    else if( errstream == true){
	cerr << "    ";

	for( int i = 0 ; i < 19 ; i++){
	    cerr << setw(3) << i << " ";
	}

	cerr << endl;

	for( int j = 18 ; j >= 0 ; j--){
	    cerr << setw(2) << j << "  ";

	    for( int i = 0 ; i < 19 ; i++){
		cerr << setw(3);

		temp.x = i;
		temp.y = j;
		if( get_spot( temp) == BLACK){
		    cerr << "O";
		}
		else if( get_spot( temp) == WHITE){
		    cerr << "X";
		}
		else if( get_moves_made() == 0){ //initial... shows a * in the center
		    if( i == 9 && j == 9){
			cerr << "*";
		    }
		    else{
			cerr << ".";
		    }
		}
		else if( get_moves_made() == 2){ //on white's second move shows a * box indicating legal moves
		    if( (i == 6 && ( j >= 6 && j <= 12))
		     || (i == 12 && ( j >= 6 && j <= 12))
		     || ((i >= 6 && i <= 12) && j == 6)
		     || ((i >= 6 && i <= 12) && j == 12 )){

			cerr << "*";
		    }
		    else{
			cerr << ".";
		    }
		}
		else{
		    cerr << ".";
		}
		cerr << " ";
	    }

	    cerr << endl << endl;
	}

	cerr << "Moves made: " << get_moves_made();
	cerr << " White's captures: " << get_white_captures();
	cerr << " Black's captures: " << get_black_captures();

	if( turn == WHITE){
	    cerr << "\t\tWhite's turn. ";
	}
	else{
	    cerr << "\t\tBlack's turn. ";
	}

	if(game_over == true){
	    cerr << "\t\tThe game is over.";
	}

	cerr << endl;
    }
}

bool Board::is_game_over_helper(){
    bool to_return = false;

    if( moves_made < 9){ //win cannot possibly exist
	to_return = false;
    }
    else if( white_captures >= 5){
	winner = WHITE;
	to_return = true;
    }
    else if( black_captures >= 5){
	winner = BLACK;
	to_return = true;
    }
    else{ //check for 5 in a rows involving the most recent move
	player temp_player = -turn; //is_game_over_helper is always called after turn is updated
	int temp_x, temp_y;

	for( int i = -1 ; i <= 1 ; i++){
	    for( int j = -1 ; j <= 1 ; j++){ //denoted direction (this could possibly be make more efficient by
					     //checking only 4 directions instead of 8. Priority: low

		temp_x = moves[ moves_made - 1].x;
		temp_y = moves[ moves_made - 1].y; //temp_x/y represent most recent moves

		while(    ( i != 0 || j != 0) //make sure a valid direction is declared
		       && (temp_x - i >= 0 && temp_x - i <= 18 && temp_y >= 0 && temp_y <= 18 ) //bounds check
		       && (spot[ temp_x - i][ temp_y - j] == temp_player) ){
		    temp_x -= i;
		    temp_y -= j;
		} //temp_x/y are now at the end of chain

		if( (i != 0 || j != 0) && //again, valid direction check
		    !(temp_x + (4 * i) < 0 || temp_x + (4 * i) > 18
		    ||temp_y + (4 * j) < 0 || temp_y + (4 * j) > 18)){ //bounds check

		    to_return = true; //flipping this flag with the expectation that it will be flipped
				      //back off if any of the following 5 checks fails
//cout << endl << endl << "b: " << endl;
		    for( int k = 1 ; k < 5 ; k++){ //starting with k == 1 because k == 0 has been checked in the while loop
//cout << "temp_x: " << temp_x << " temp_y: " << temp_y << " i: " << i << " j: " << j << endl;
//cout << "spot[" << temp_x + (k*i) << "][" << temp_y + (k * j) << "] : " << spot[ temp_x + (k * i)][ temp_y + (k * j)] << " temp_player: " << temp_player << endl << endl;
			if( spot[ temp_x + (k * i)][ temp_y + (k * j)] != temp_player){
			    to_return = false;
			    break;
			}
		    }

		    if( to_return == true){
			winner = temp_player;
			return to_return;
			//goto found;
		    }
		}
	    }
	}
    }
    //found:

    return to_return;
}

void Board::clear_board_helper(){ //DOES NOT CLEAR moves_x/y
    for( int i = 0 ; i < 19 ; i++){
	for( int j = 0 ; j < 19 ; j++){
	    spot[i][j] = EMPTY;
	}
    }

    moves_made = 0;
    turn = WHITE;
    game_over = 0;
    winner = EMPTY;
    white_captures = 0;
    black_captures = 0;
}

void Board::perform_captures_helper(player the_player, Move_t a){
  //This should only be called from make_move() and the legality is checked there first
    int x = a.x;
    int y = a.y;

    for( int i = -1 ; i <= 1 ; i++){
	for( int j = -1 ; j <= 1 ; j++){ //cardinal directions

	    if( x + (3 * i) < 0 || x + (3 * i) > 18 || y + (3 * j) < 0 || y + (3 * j) > 18 || (i == 0 && j == 0)) //out of bounds and center check
		continue;

	    if( spot[ x + (3 * i)][ y + (3 * j)] == the_player && spot[ x + (2 * i)][y + (2 * j)] == -the_player && spot[ x + i][y + j] == -the_player){
		spot[ x + (2 * i)][ y + (2 * j)] = EMPTY;
		spot[ x + i][ y + j] = EMPTY;

		if( the_player == WHITE){
		    white_captures++;
		}
		else{
		    black_captures++;
		}
	    }
	}
    }
}

bool Board::move_is_legal(player the_player, Move_t a){
    bool to_return;
    int x = a.x;
    int y = a.y;

    if( !(moves_made % 2 == 0 && the_player == WHITE)
      &&!(moves_made % 2 == 1 && the_player == BLACK)){
	to_return = false;
    }
    else if( x < 0 || x >= 19 || y < 0 || y >= 19){ //out of bounds
	to_return = false;
    }
    else if( spot[x][y] != EMPTY){ //spot occupied
	to_return = false;
    }
    else if( get_game_over()){ //game is over
	to_return = false;
    }
    else if( moves_made == 0 && ( x != 9 || y != 9)){ //first turn rule
	to_return = false;
    }
    else if( moves_made == 2 && ( x > 6 && x < 12 && y > 6 && y < 12)){ //white's second turn rule
	to_return = false;
    }
    else{
	to_return = true;
    }

    return to_return;
}

bool Board::make_move(player the_player, Move_t a){

    if( move_is_legal( the_player, a)){
	int x = a.x;
        int y = a.y;

	spot[x][y] = the_player;

	moves.push_back(a);
	moves_made++;

	if( turn == WHITE){
	    turn = BLACK;
	}
	else{
	    turn = WHITE;
	}

	game_over = is_game_over_helper(); //must call after update to turn
	perform_captures_helper( the_player, a);

	return true;
    }
    else{
	return false;
    }
}

int Board::get_turn(){
    return turn;
}

player Board::get_spot(Move_t move_to_check){
    return spot[ move_to_check.x][ move_to_check.y];
}

int Board::get_moves_made(){
    return moves_made;
}

bool Board::get_game_over(){
    //game_over = is_game_over_helper(); //make sure this is updated

    return game_over;
}

int Board::get_winner(){
    //game_over = is_game_over_helper(); //make sure game_over is updated. The helper function
				       //updates who the winner is if game is over

    return winner;
}

int Board::get_white_captures(){
    return white_captures;
}

int Board::get_black_captures(){
    return black_captures;
}

player Board::get_player(){
    if( moves_made % 2 == 0){
	return WHITE;
    }
    else{
	return BLACK;
    }
}

Move_t Board::get_move(int n){
    if( n < moves_made){
	return moves[n];
    }
    else{
	cout << "error... invalid move requested." << endl;
	exit(-1);
    }
}
