CC=g++
CFLAGS=-c -Wall -O2
LDFLAGS=
SOURCES=src/main.cpp src/training.cpp src/Strategy.cpp src/Blackjack.cpp src/State.cpp include/DefaultDict.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=blackjack

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@