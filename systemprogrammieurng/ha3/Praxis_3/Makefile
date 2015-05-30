BIN = scheduler
# Insert additional sources below
# - order matters, compile requirements first
# - end each line with a '\' to avoid errors
SRC = \
	PrioQueue.c \
	task.c \
	LCFS.c \
	SRTN.c \
	SJN.c \
	MLF.c \
	main.c

CC = cc
CFLAGS = -g -Wall -Wextra -Werror -O2 -std=c99
LFLAGS = -lm

GDB = gdb
RM = rm -f

OBJS = $(SRC:%.c=%.o)

# type make all (or simply make) to compile all Source-files
all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $@

# pull in dependencies for .o files
-include $(OBJS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
	$(CC) -MM $(CFLAGS) $*.c > $*.d

# type make debug to debug your programm with gdb
debug: $(BIN)
	$(GDB) ./$(BIN)

# type make run to execute your programm
run: $(BIN)
	./$(BIN)

# type make clean to remove object and dependency files
clean:
	$(RM) $(BIN) $(OBJS) *.d

.PHONY: clean debug run
