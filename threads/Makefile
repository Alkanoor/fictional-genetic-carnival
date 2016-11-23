OBJ_DIR = obj
SRC_DIR = .
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC)))
VPATH=.

CC = g++
CFLAGS = -Wall -Werror -O1 -std=c++11 -fPIC -I..


all: createDir $(OBJ)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

createDir:
	@mkdir -p $(OBJ_DIR)

clean :
	@rm -rf $(OBJ_DIR)
