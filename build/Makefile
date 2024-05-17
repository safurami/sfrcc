CXX := g++
CXXFLAGS := -Wextra -Wall -pedantic -c -g -std=c++17
LN := g++

SRCD := ../src/
SRC := $(wildcard $(SRCD)*.cpp)

OBJ := $(patsubst $(SRCD)%.cpp, %.o, $(SRC))


EXECUTABLE := ../main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	@echo "LN\t$@"
	@$(LN) $(OBJ) -o $@

$(OBJD)%.o: $(SRCD)%.cpp
	@echo "CXX\t$^"
	@$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@rm -f $(OBJ) $(EXECUTABLE)
