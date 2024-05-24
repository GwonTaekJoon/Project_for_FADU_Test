CC = gcc
TARGET = SHA512_verification
SRC = ./main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC)


clean:
	rm -f $(TARGET)

.PHONY: clean
