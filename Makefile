CC = g++
LN = g++
CCFLAGS = -c -std=c++17 -pedantic -Wall -Werror

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp, obj/%.o, $(SRC))

TARGET = main

default: $(TARGET)

debug: CCFLAGS += -g
debug: $(TARGET)

$(TARGET): $(OBJ)
	$(LN) $(OBJ) -o $@

obj/%.o: src/%.cpp
	$(CC) $(CCFLAGS) $^ -o $@

clean:
	rm -f obj/* $(TARGET)
