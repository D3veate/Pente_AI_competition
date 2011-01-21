#ifndef PENTE_COM_H
#define PENTE_COM_H

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include "pente_definitions.h"

class Pente_Com{
    public:
	Pente_Com();
	~Pente_Com();

	player get_color();
	void play_move( Move);
	Move get_opponents_move();
    protected:

    private:
	int n_read;
	int n_write;
	player color;
};

#endif
