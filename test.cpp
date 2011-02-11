#include "pente_definitions.h"
#include "board.h"
#include "pente_com.h"
#include "pente_ai.h"

int main()
{
    Board the_board;
    Pente_AI the_ai;
    char* temp = "123,456,1:4";
    cerr << "target: " << temp << endl;
    cerr << "This: " << atoi( temp) << endl;
    cerr << "That: " << atoi( &temp[4]) << endl;
    cerr << "and the: " << atoi( &temp[8]) << endl;
    cerr << "other: " << atoi( &temp[10]) << endl;

    the_ai.simple_rules_ai( the_board);

    return 0;
}
