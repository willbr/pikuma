
SRC_DIR           := src
BIN_DIR           := bin
SRC_BUILD_DIR     := src/build
BUILD_DIR         := build

CC          := clang
TOCI_FLAGS    := --map-lines --map-filenames
#CFLAGS := -v
CFLAGS := -DSDL_MAIN_HANDLED
IFLAGS := -I$(SRC_DIR) -I$(SRC_BUILD_DIR)
WATCH       := watchexec --restart --debounce 3000


ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    detected_OS := Windows
	mkdir = mkdir
	make = make
	rm = del
	cp = copy
	EXE = .exe
	SDL = C:\tools\SDL\SDL2-2.0.20\x86_64-w64-mingw32
	SDL_FLAGS = -I$(SDL)\include -L$(SDL)\lib -lSDL2
else
    detected_OS := $(shell uname)  # same as "uname -s"
	mkdir = mkdir
	make = make
	rm = rm
	cp = cp
	EXE =
	SDL_FLAGS = 
endif

TOCI := "./$(BIN_DIR)/toci$(EXE)"

all: create-window

#######################################################

$(TOCI): $(BIN_DIR)
	-$(cp) "..\..\ie\build\toci$(EXE)" $(BIN_DIR)

#######################################################

$(BIN_DIR):
	-$(mkdir) "$(BIN_DIR)"

$(BUILD_DIR):
	-$(mkdir) "$(BUILD_DIR)"

$(SRC_BUILD_DIR):
	-$(mkdir) "$(SRC_BUILD_DIR)"

#######################################################

$(SRC_BUILD_DIR)/%.c: $(SRC_DIR)/%.c.ie $(TOCI) | $(SRC_BUILD_DIR) $(TOCI)
	$(TOCI) $(TOCFLAGS) $< -o $@

$(SRC_BUILD_DIR)/%.h: $(SRC_DIR)/%.h.ie $(TOCI) | $(SRC_BUILD_DIR) $(TOCI)
	$(TOCI) $(TOCFLAGS) $< -o $@

#######################################################

create-window: $(SRC_BUILD_DIR)/00-create-window.c
	$(CC) $(CFLAGS) $(IFLAGS) $(SDL_FLAGS) -v -o $@ $<

w-create-window:
	$(WATCH) "$(make) create-window"

