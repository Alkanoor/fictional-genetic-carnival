SRC_DIRS = ../../logger ../../threads ../../utils ../../algorithm
OBJS = $(addsuffix /obj/*.o, $(SRC_DIRS))
SRC = test.cpp

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -I../..
LDFLAGS = -lpthread

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
