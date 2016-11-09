OBJ_DIR = obj
SRC_DIR = .
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC)))
VPATH=.

RELEASE_DIR = bin
EXEC = $(RELEASE_DIR)/test
LIB = $(RELEASE_DIR)/liblog.so

CC = g++
CFLAGS = -Wall -Werror -O1 -std=c++11 -fPIC
LDFLAGS = -fPIC
LDFLAGS_LIB = -fPIC -shared

LOG_DIRS = logs logs/uselessDir logs/uselessDirBis


all: createDir $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

test: all
	./$(EXEC)

lib: createDir $(OBJ)
	$(CC) -o $(LIB) $(OBJ) $(LDFLAGS_LIB)

createDir:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(RELEASE_DIR)
	@mkdir -p $(LOG_DIRS)

clean :
	@rm -rf $(OBJ_DIR)
	@rm -rf $(RELEASE_DIR)
	@rm -rf $(LOG_DIRS)
