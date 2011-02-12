#ifndef PENTE_DEFINITIONS_H
#define PENTE_DEFINITIONS_H

using namespace std;

typedef int player;

enum{
    BLACK = -1,
    EMPTY = 0,
    WHITE = 1
};

typedef struct Move_t{
    int x;
    int y;
}Move_t;

#endif
