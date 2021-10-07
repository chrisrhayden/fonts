CC=gcc
CFLAGS=-Wall -g -pedantic

SRC = $(wildcard ./src/*.c) \
# $(wildcard ./src/font/*.c)

TARGET_NAME = ./out/target_build

LIB_FLAGS = -lm `pkg-config --cflags --libs freetype2`

build: $(SRC)
	$(CC) $(CFLAGS) $^ $(LIB_FLAGS) -o $(TARGET_NAME)

run: build
	./out/target_build
