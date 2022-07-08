CC = g++
CXXFLAGS = -std=c++11 -g -Wall


INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

MAIN = Main
TARGET = tp01.out
SRC = $(wildcard $(SRC_FOLDER)*.cpp)
OBJ = $(patsubst $(SRC_FOLDER)%.cpp, $(OBJ_FOLDER)%.o, $(SRC))

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER)

build: $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN_FOLDER)$(TARGET) $(OBJ)

clean:
	@rm -rf $(OBJ_FOLDER)*
