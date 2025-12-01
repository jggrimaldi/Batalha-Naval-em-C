CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
SRCS = main.c board.c fleet.c game.c io.c rnd.c
OBJS = $(SRCS:.c=.o)
TARGET = battleship

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c game.h io.h rnd.h

board.o: board.c board.h

fleet.o: fleet.c fleet.h

game.o: game.c game.h board.h fleet.h io.h rnd.h

io.o: io.c io.h board.h

rnd.o: rnd.c rnd.h

clean:
	rm -f $(OBJS) $(TARGET)