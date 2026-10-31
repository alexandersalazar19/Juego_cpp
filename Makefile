CXX=g++
CXXFLAGS=-Wall -Wextra -MMD -MP -Isrc
LDLIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC=$(shell find src -name '*.cpp')
OBJ=$(SRC:src/%.cpp=build/%.o)
DEP=$(OBJ:.o=.d)

juego: $(OBJ)
	$(CXX) $(OBJ) -o juego $(LDLIBS)

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: juego
	./juego

clean:
	rm -rf build juego
	clear

.PHONY: run clean

-include $(DEP)