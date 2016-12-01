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
CFLAGS = -D_GLIBCXX_USE_CXX11_ABI=0 -Wabi-tag -Wall -Werror -Wextra -O1 -std=c++11 -fPIC
LDFLAGS = -D_GLIBCXX_USE_CXX11_ABI=0 -lpthread -lstdc++ -fPIC
LDFLAGS_LIB = -D_GLIBCXX_USE_CXX11_ABI=0 -lpthread -lstdc++ -fPIC -shared

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
