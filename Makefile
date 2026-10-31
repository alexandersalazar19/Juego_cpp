CXX=g++
CXXFLAGS=-Wall -Wextra -MMD -MP
LDLIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

juego: $(OBJ)
	$(CXX) $(OBJ) -o juego $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: juego
	./juego

clean:
	rm -f $(OBJ) *.d juego
	clear

.PHONY: run clean