CC = gcc

CFLAGS = -Wall -Werror -g -I$(HEAD_DIR)

SRC_DIR = src
OBJ_DIR = o
HEAD_DIR = head

MODULES = GameState queue display

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