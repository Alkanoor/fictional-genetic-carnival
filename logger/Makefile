OBJ_DIR = obj
OBJ_DIR_TEST = obj/tests
SRC_DIR = .
SRC_DIR_TEST = tests
SRC = $(wildcard $(SRC_DIR)/*.cpp)
SRC_TEST = $(wildcard $(SRC_DIR_TEST)/*.cpp)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC)))
OBJ_TEST := $(patsubst %.cpp, $(OBJ_DIR_TEST)/%.o, $(notdir $(SRC_TEST)))

VPATH = .

RELEASE_DIR = bin
EXEC = $(RELEASE_DIR)/test
LIB = $(RELEASE_DIR)/liblog.so

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -fPIC
LDFLAGS = -lpthread -fPIC
LDFLAGS_LIB = -lpthread -fPIC -shared

LOG_DIRS = logs logs/uselessDir logs/uselessDirBis


all: createDir $(EXEC)

$(EXEC): $(OBJ_TEST) $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR_TEST)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

test: all
	./$(EXEC)

lib: createDir $(OBJ)
	$(CC) -o $(LIB) $(OBJ) $(LDFLAGS_LIB)

obj: createDir $(OBJ)

createDir:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR_TEST)
	@mkdir -p $(RELEASE_DIR)
	@mkdir -p $(LOG_DIRS)

clean :
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR_TEST)
	@rm -rf $(RELEASE_DIR)
	@rm -rf $(LOG_DIRS)
