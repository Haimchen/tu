BIN = deadlock
SRC = \
	deadlock.c \
	print.c \

OBJECTS = $(SRC:%.c=%.o)

CC	    = gcc
CFLAGS  = -std=gnu99 -Wall -Wextra -Werror -DCOLOR
LFLAGS  = -lpthread -lrt

RM      = rm -f

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LFLAGS) -o $@

-include $(OBJS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
	$(CC) -MM $(CFLAGS) $*.c > $*.d

debug: CFLAGS += -DDEBUG -g
debug: $(BIN)

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(BIN) $(OBJECTS) *.d

.PHONY: clean debug run 
