#include "pente_definitions.h"
#include "board.h"
#include "pente_com.h"
#include "pente_ai.h"

int main()
{
    Board the_board;
    Pente_AI the_ai;
    the_ai.simple_rules_ai( the_board);

    return 0;
}
