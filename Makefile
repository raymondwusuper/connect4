CC = gcc
RM = rm -f 
CFLAGS = -g -lm

SOURCES = connect4.c connect4_functions.c display_board_ASCII.c

all: connect4


# The main executable
connect4: $(SOURCES)
	$(CC)  $(SOURCES) $(CFLAGS) -o $@


# Cleaning all object files and executables
clean:
	@$(RM) connect4 connect4_test connect4_demo connect4_check connect4_test_competition connect4_compete *.o 
