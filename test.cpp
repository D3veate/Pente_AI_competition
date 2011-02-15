#include "pente_definitions.h"
#include "board.h"
#include "pente_com.h"
#include "pente_ai.h"

int main()
{
    Board the_board;
    Pente_AI the_ai;

    for( int i = 0 ; i < 10 ; i ++){
	the_board.make_move( WHITE, the_ai.random_ai( the_board));
	the_board.make_move( BLACK, the_ai.random_ai( the_board));
    }

    the_board.display();
    Move_t temp = the_ai.simple_rules_ai( the_board);
    cerr << "Projected move-- x: " << temp.x << " y: " << temp.y << endl;

    return 0;
}
