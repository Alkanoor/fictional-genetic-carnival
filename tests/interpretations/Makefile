SRC_DIR1 = ../../genetic
SRC_DIR2 = ../../logger
SRC_DIRS = $(SRC_DIR1) $(SRC_DIR2)
OBJS = $(wildcard $(SRC_DIR1)/obj/*.o) $(wildcard $(SRC_DIR2)/obj/*.o)
SRC = test.cpp

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -I../..
LDFLAGS =

EXEC = test

.PHONY: $(SRC_DIRS)

$(EXEC): $(SRC_DIRS) $(OBJ)
	$(CC) -o $@ $(OBJS) $(SRC) $(CFLAGS) $(LDFLAGS)

$(SRC_DIRS):
	make -C $@ obj

run: $(EXEC)
	./$(EXEC)

clean :
	@rm -rf $(EXEC)
