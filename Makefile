CXX := g++
CXXFLAGS := -Wall -pedantic -c -g
LN := g++

SRCD := ./src/
SRC := $(wildcard $(SRCD)*.cpp)

OBJD := ./obj/
OBJ := $(patsubst $(SRCD)%.cpp, $(OBJD)%.o, $(SRC))


EXECUTABLE := main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	@echo "LN\t$@"
	@$(LN) $(OBJ) -o $@

$(OBJD)%.o: $(SRCD)%.cpp
	@echo "CXX\t$^"
	@$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@rm -f $(OBJ) $(EXECUTABLE)
