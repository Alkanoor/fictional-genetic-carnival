SRC_DIRS = ../../genetic ../../logger ../../threads ../../utils
OBJS = $(addsuffix /obj/*.o, $(SRC_DIRS))
SRC = test.cpp

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -I../..
LDFLAGS = -lpthread

EXEC = test


.PHONY: $(SRC_DIRS)

$(EXEC): $(SRC_DIRS)
	$(CC) -o $@ $(OBJS) $(SRC) $(CFLAGS) $(LDFLAGS)

$(SRC_DIRS):
	make -C $@ obj

run: $(EXEC)
	./$(EXEC)

clean :
	@rm -rf $(EXEC)
