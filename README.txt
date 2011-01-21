Pente_AI_competition README

--What is it--
A challenge from around 2006 from JEvans. The challenge is that each person
creates a pente AI that plays against others' AI's.

--What the code in this repository is/does--
board.h
Board.cpp: The Board class is written in C++ and is intended as a starting
point for a pente program. I encourage anyone who wants to use the Board
class to use it in their code, as it shortens the tedium of creating the
framework that an AI needs in order to operate.

pente_ai.h
pente_ai.cpp: This class is intended to be a collection of different AI's
that I have created. As of 1/20/2011 the AI picks a random (but legal) move. 
It is currently the champion.

info.txt
main.cpp
pente_com.h
Pente_Com.cpp: The main.cpp code creates a program that will check the two
programs listed in info.txt. It will then spawn those programs using child
processes, the dup2() function, and the execl() function in order to set up
a method of communication. The parent program coordinates the game that the
two AI's play. This method of communication assumes that the two AI's are 
using the Pente_Com class to communicate. The Pente_Com class and main.cpp
are more or less hard coded to work together. Keep in mind: any AI spawned 
by main.cpp should not use stdio or stdout, as these two streams are 
hijinxed by the communication pipes with main.cpp.

dummy.out
pente.out: These two files shouldn't be here. Hmm. They are the files that
are created when make works on teh two makefiles

Makefile
makedummy: These two makefiles create local compilations of pente.out and
dummy.out. The program pente.out is created from the code in main.cpp and 
will spawn the two programs listed info.txt. The program dummy.out is an
example of my random AI.

pente_definitions.h: This file defines the Move struct, typedefs an int as
a player, and enums WHITE, EMPTY, and BLACK.


Note: JEvans has a pente server that, by all accounts, is amazing. I'm hoping
that when I grow up and gain wisdom, I can modify the Pente_Com class so that
it can communicate using his pente server if the server is available.
