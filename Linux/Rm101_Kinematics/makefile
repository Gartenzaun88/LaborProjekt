CC	= gcc
#Um DEBUG auszuschalten die 2 Lines durch die auskommentierten ersetzen
#CFLAGS  = -Wall -Wextra -pedantic -DDEBUG
CFLAGS  = -Wall -Wextra -pedantic
LDFLAGS = -lm
BIN = prog
NAME = libRm101Kinematics
OBJ = Rm101Kinematics.o
#FILE = Debug
FILE = Output

prog: $(OBJ)
	ar rs $(FILE)/$(NAME).a $(OBJ) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ) $(FILE)/$(NAME).a
