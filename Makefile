VERSION=1
CC=g++
CFLAGS=-g -Wall
A_HEADERS=pente_definitions.h board.h pente_com.h pente_ai.h
A_OBJECTS=Board.o Pente_Com.o Pente_AI.o
A_CPP=Board.cpp Pente_Com.cpp Pente_AI.cpp

all: pente.out test.out dummy.out logan_pente_ai_$(VERSION).out

pente.out: $(A_HEADERS) main.cpp Board.o
	$(CC) $(CFLAGS) main.cpp Board.o -o pente.out

test.out: $(A_HEADERS) $(A_CPP) $(A_OBJECTS) test.cpp
	$(CC) $(CFLAGS) test.cpp $(A_OBJECTS) -o test.out

dummy.out: $(A_HEADERS) $(A_CPP) $(A_OBJECTS) dummy.cpp
	$(CC) $(CFLAGS) dummy.cpp $(A_OBJECTS) -o dummy.out

logan_pente_ai_$(VERSION).out: logan_pente_ai_$(VERSION) $(A_CPP) $(A_OBJECTS)
	$(CC) $(CFLAGS) logan_pente_ai_$(VERSION).cpp $(A_OBJECTS) -o logan_pente_ai_$(VERSION).out

Pente_Com.o: pente_definitions.h pente_com.h Pente_Com.cpp
	$(CC) $(CFLAGS) -c Pente_Com.cpp -o Pente_Com.o

Pente_AI.o: pente_definitions.h pente_ai.h Pente_AI.cpp board.h Board.cpp Board.o
	$(CC) $(CFLAGS) -c Pente_AI.cpp -o Pente_AI.o
# Pente_AI.cpp refers to board.h, so I want Board.o and associated changes to trigger a recompile. 
# if the line reads: 
#	$(CC) $(CFLAGS) Board.o Pente_AI.cpp -o Pente_AI.o
# then the error is that in function _start I have un undefined reference to main. 
# With the rule: 
#	$(CC) $(CFLAGS) -c Pente_AI.cpp -o Pente_AI.o

Board.o: pente_definitions.h board.h Board.cpp
	$(CC) $(CFLAGS) -c Board.cpp -o Board.o

clean:
	rm -f *.out
	rm -f *.o
