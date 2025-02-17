#FLAGS=-Wall -Werror -lm -g

#7color : o/GameState.o
#	gcc $(FLAGS) o/GameState.o -o 7color

#o/GameState.o: head/GameState.h src/GameState.c
#	gcc $(FLAGS) -c src/GameState.c -o o/GameState.o


CC = gcc

CFLAGS = -Wall -g -I$(HEAD_DIR)

SRC_DIR = src
OBJ_DIR = o
HEAD_DIR = head

MODULES = GameState queue

OBJ = $(patsubst %, $(OBJ_DIR)/%.o, $(MODULES))

TARGET = 7color

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

cleanall: clean
	rm -rf $(OBJ_DIR)