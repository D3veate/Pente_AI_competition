VERSION=1
CC=g++
CFLAGS=-g -Wall
A_HEADERS=pente_definitions.h board.h pente_com.h pente_ai.h
A_OBJECTS=Board.o Pente_Com.o Pente_AI.o
A_CPP=Board.cpp Pente_Com.cpp Pente_AI.cpp

all: pente.out test.out human_player.out logan_pente_ai_$(VERSION).out

pente.out: main.cpp Board.o
	$(CC) $(CFLAGS) main.cpp Board.o -o pente.out

test.out: $(A_OBJECTS) test.cpp
	$(CC) $(CFLAGS) test.cpp $(A_OBJECTS) -o test.out

#dummy.out: $(A_OBJECTS) dummy.cpp
#	$(CC) $(CFLAGS) dummy.cpp $(A_OBJECTS) -o dummy.out

human_player.out: human_player.cpp Board.o Pente_AI.o
	$(CC) $(CFLAGS) human_player.cpp Pente_AI.o Board.o -o human_player.out

logan_pente_ai_$(VERSION).out: logan_pente_ai_$(VERSION).cpp $(A_OBJECTS)
	$(CC) $(CFLAGS) logan_pente_ai_$(VERSION).cpp $(A_OBJECTS) -o logan_pente_ai_$(VERSION).out

Pente_Com.o: pente_definitions.h pente_com.h Pente_Com.cpp
	$(CC) $(CFLAGS) -c Pente_Com.cpp -o Pente_Com.o

Pente_AI.o: pente_definitions.h pente_ai.h Pente_AI.cpp Board.o
	$(CC) $(CFLAGS) -c Pente_AI.cpp -o Pente_AI.o

Board.o: pente_definitions.h board.h Board.cpp
	$(CC) $(CFLAGS) -c Board.cpp -o Board.o

clean:
	rm -f *.out
	rm -f *.o
