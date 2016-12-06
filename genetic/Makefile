OBJ_DIR = obj
SRC_DIR = .
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC)))

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -I..


all: createDir $(OBJ)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

obj: createDir $(OBJ)

createDir:
	@mkdir -p $(OBJ_DIR)

clean :
	@rm -rf $(OBJ_DIR)
