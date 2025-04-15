CC = gcc

CFLAGS = -Wall -Werror -g -I$(HEAD_DIR) -fopenmp
LDFLAGS = -lm -lSDL2_ttf -lSDL2


SRC_DIR = src
OBJ_DIR = o
HEAD_DIR = head

MODULES = GameState queue display Agents utilities map_functions main

OBJ = $(patsubst %, $(OBJ_DIR)/%.o, $(MODULES))

TARGET = 7color_visuel

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

cleanall: clean
	rm -rf $(OBJ_DIR)