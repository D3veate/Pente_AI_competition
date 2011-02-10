#ifndef PENTE_AI_H
#define PENTE_AI_H

#include "pente_definitions.h"
#include "board.h"
#include <vector>
#include <string.h>
#include <sstream>

class Pente_AI{
    private:
	bool rules_initialized;
	void initialize_rules( int = 0);
	vector< vector< string> > rules;

    public:
	Pente_AI();
	void search_rectangle(Board&, int&, int&, int&, int&);
	Move random_ai(Board&);
	Move narrow_random_ai(Board&);
	Move simple_rules_ai(Board&);
};

#endif
