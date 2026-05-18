CC = gcc

CFLAGS = -Wall -Wextra -Iinclude -g

SRC = $(wildcard src/*.c)

HDR = $(wildcard include/*.h)

TARGET = mini_mux

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)


clean:
	rm -f $(TARGET)
