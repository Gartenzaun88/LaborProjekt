CC	= gcc
CFLAGS  = -Wall -Wextra -pedantic
LDFLAGS = -Llib -lRm101Control
BIN = prog
NAME = MoveRandom
OBJ = $(NAME).o 

prog: $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ)
