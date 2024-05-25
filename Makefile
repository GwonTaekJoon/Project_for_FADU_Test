CC = gcc
TARGET = SHA512_verification
SRC = ./main.c
LDFLAGS = -lssl -lcrypto

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(LDFLAGS)


clean:
	rm -f $(TARGET)

.PHONY: clean
