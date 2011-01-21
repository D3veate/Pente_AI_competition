#ifndef PENTE_AI_H
#define PENTE_AI_H

#include "pente_definitions.h"
#include "board.h"

class Pente_AI{
    private:
	void search_rectangle(Board&, int&, int&, int&, int&);
    public:
	Move random_ai(Board&);
};

#endif
