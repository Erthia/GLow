CC = gcc
CFLAGS = -Wall -O2 -g
LDFLAGS = -lSDL -lm
LIB = -lSDL -lGLU -lGL -lm -lSDL_image

APP_BIN = GLow

SRC_PATH = src
OBJ_PATH = obj
INC_PATH = -I include
BIN_PATH = bin

SRC_FILES = $(shell find $(SRC_PATH) -type f -name '*.c')
OBJ_FILES = $(patsubst $(SRC_PATH)/%.c,$(OBJ_PATH)/%.o, $(SRC_FILES))

all: $(APP_BIN)

$(APP_BIN): $(OBJ_FILES)
	@mkdir -p $(BIN_PATH)
	$(CC) -o $(BIN_PATH)/$(APP_BIN) $(OBJ_FILES) $(LIB) $(LDFLAGS) 

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p "$(@D)"
	$(CC) -c $< -o $@ $(CFLAGS) $(INC_PATH)

clean:
	rm $(OBJ_FILES) $(BIN_PATH)/$(APP_BIN)
