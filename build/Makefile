CXX := g++
CXXFLAGS := -Wextra -Wall -pedantic -c -std=c++17
LN := g++

SRCD := ../src/
SRC := $(wildcard $(SRCD)*.cpp)

OBJ := $(patsubst $(SRCD)%.cpp, %.o, $(SRC))

EXECUTABLE := main
default: debug

debug: CXXFLAGS += -DDEBUG -g
debug: $(EXECUTABLE)

release: $(EXECUTABLE)

clean:
	rm -f $(OBJ) ../$(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
#	@echo "ln\t$@"
	@$(LN) $(OBJ) -o ../$@

$(OBJD)%.o: $(SRCD)%.cpp
#	@echo "cxx\t$^"
	@$(CXX) $(CXXFLAGS) $^ -o $@

