PYTHON_INC_PATH=python -c 'from sysconfig import get_paths; print(get_paths()["include"])'
PYTHON_INC =$(shell $(PYTHON_INC_PATH))

SRC_DIR = src
OBJ_DIR = o
HEAD_DIR = head


MODULES_PYTHON = 7color-python


MODULES = GR0_GameState GR0_queue GR0_display GR0_Agents GR0_utilities GR0_map_functions GR0_main GR0_sdl GR0_console

CC = gcc

PYFLAGS=-fPIC
CFLAGS = -Wall -Werror -g -I$(HEAD_DIR) -fopenmp
LDFLAGS = -lm -lSDL2_ttf -lSDL2

OBJ = $(patsubst %, $(OBJ_DIR)/%.o, $(MODULES))
PYOBJ = $(patsubst %, $(OBJ_DIR)/%.o, $(MODULES_PYTHON))

TARGET = 7color

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(PYOBJ)
	$(CC) $(CFLAGS) $(PYOBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.so: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@



verlet.so : verlet.c
	gcc --shared -I${PYTHON_INC} $(PYFLAGS) verlet.c -o verlet.so

clean:
	rm -f $(TARGET) $(OBJ)

cleanall: clean
	rm -rf $(OBJ_DIR)