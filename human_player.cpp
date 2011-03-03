#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "pente_definitions.h"
#include "board.h"
#include "pente_ai.h"
using namespace std;

int main(){

    Pente_AI the_AI;
    Board the_board;
    player human_color;
    player op_color;
    Move_t temp;

    char choice = ' ';
    while( choice == ' '){

	cout << "Do you want to play was white (w) or black (b)?";
	cin >> choice;

	if( choice == 'w' || choice == 'W')
	    human_color = WHITE, op_color = BLACK;
	else if( choice == 'b' || choice == 'B')
	    human_color = BLACK, op_color = WHITE;
	else{
	    cout << "Invalid choice." << endl;
	    choice = ' ';
	}

	cout << "test: " << human_color << " : " << op_color << endl;
    }

    if( human_color == BLACK){
	temp = the_AI.simple_rules_ai( the_board, 1);
	the_board.make_move( op_color, temp);
    }

    string to_move_x, to_move_y;
    Move_t temp_display;
    while( !the_board.get_game_over()){

	the_board.display();
	if( the_board.get_moves_made() > 0){
	    temp_display = the_board.get_move( the_board.get_moves_made() - 1);
	    cout << "Last move -- x: " << temp_display.x << " y: " << temp_display.y << endl;
	}

	do{
	    temp.x = -1;
	    temp.y = -1;
	    cout << "Your move -- x: ";
	    cin >> to_move_x;
	    temp.x = atoi( to_move_x.c_str());
	    cout << "             y: ";
	    cin >> to_move_y;
	    temp.y = atoi( to_move_y.c_str());
	}while( !the_board.move_is_legal( human_color, temp));

	the_board.make_move( human_color, temp);

	if( !the_board.get_game_over()){
	    temp = the_AI.simple_rules_ai( the_board, 1);
	    the_board.make_move( op_color, temp);
	}
    }

    cout << "The winner is ";
    if( the_board.get_winner() == human_color){
	cout << "the human!" << endl;
    }
    else{
	cout << "the computer! Sucker." << endl;
    }

    return 0;
}
