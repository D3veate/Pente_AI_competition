version=1
CC=g++
CFLAGS=-Wall -g

all: pente.out test.out dummy.out logan_pente_ai_$(version).out

pente.out: main.cpp Board.cpp
	$(CC) -Wall -g main.cpp Board.cpp -o pente.out

test.out: test.cpp Board.cpp Pente_Com.cpp Pente_AI.cpp
	g++ -Wall -g test.cpp Board.cpp Pente_Com.cpp Pente_AI.cpp -o test.out

dummy.out: dummy.cpp Board.cpp Pente_Com.cpp Pente_AI.cpp
	g++ -Wall -g test.cpp Board.cpp Pente_Com.cpp Pente_AI.cpp -o dummy.out

logan_pente_ai_$(version).out: dummy.cpp Board.cpp Pente_Com.cpp Pente_AI.cpp
	g++ -Wall -g logan_pente_ai_1.cpp Board.cpp Pente_Com.cpp Pente_AI.cpp -o logan_pente_ai_$(version).out
