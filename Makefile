CXX := g++
CXXFLAGS := -Wall -pedantic -g -c
LN := g++

OBJ = ./obj/
OBJS = $(OBJ)main.o $(OBJ)lexer.o $(OBJ)my.o $(OBJ)token.o

SRC = ./src/

EXECUTABLE := main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@echo "LN\t$@"
	@$(LN) $(OBJS) -o $@

$(OBJ)main.o: $(SRC)main.cpp
	@echo "CXX\t$^"
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ)lexer.o: $(SRC)lexer.cpp
	@echo "CXX\t$^"
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ)my.o: $(SRC)my.cpp
	@echo "CXX\t$^"
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ)token.o: $(SRC)token.cpp
	@echo "CXX\t$^"
	@$(CXX) $(CXXFLAGS) $^ -o $@
clean:
	rm $(OBJS) $(EXECUTABLE)
