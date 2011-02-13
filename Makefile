version=1
CC=g++
CFLAGS=-g -Wall

all: pente.out test.out dummy.out logan_pente_ai_$(version).out

pente.out: main.cpp Board.cpp
	$(CC) $(CFLAGS) main.cpp Board.cpp -o pente.out

test.out: test.cpp Board.cpp board.h Pente_Com.cpp pente_com.h Pente_AI.cpp pente_ai.h
	$(CC) $(CFLAGS) test.cpp Board.cpp Pente_Com.cpp Pente_AI.cpp -o test.out

dummy.out: dummy.cpp Board.cpp Pente_Com.cpp Pente_AI.cpp
	$(CC) $(CFLAGS) dummy.cpp Board.cpp Pente_Com.cpp Pente_AI.cpp -o dummy.out

logan_pente_ai_$(version).out: dummy.cpp Board.cpp Pente_Com.cpp Pente_AI.cpp
	$(CC) $(CFLAGS) logan_pente_ai_1.cpp Board.cpp Pente_Com.cpp Pente_AI.cpp -o logan_pente_ai_$(version).out

clean:
	rm -f *.out
	rm -f *.o
