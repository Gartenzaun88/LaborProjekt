CC	= gcc
CFLAGS  = -Wall -Wextra -pedantic
LDFLAGS = -Llib -lRm101Kinematics -lRm101Control -lm
BIN = prog
NAME = MoveXYZ
OBJ = $(NAME).o 

prog: $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ)
