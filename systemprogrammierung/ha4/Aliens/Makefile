BIN = aliens
SRC = \
	md5.c \
	buffer.c \
	passwd_generator.c \
	utils.c \
	passwd_cracker.c \
	passwd_decrypter.c \
	crypto.c \
	main.c	

CC	     = gcc
CFLAGS   = -g -Wall -Wextra -O3
LFLAGS   += -lm -lpthread


GDB    = gdb
RM     = rm -f

OBJS = $(SRC:%.c=%.o)

all: $(BIN)


$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $@

# pull in dependencies for .o files
-include $(OBJS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
	$(CC) -MM $(CFLAGS) $*.c > $*.d


debug: $(BIN)
	$(GDB) ./$(BIN)

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(BIN) $(OBJS) *.d

.PHONY: clean debug run 
