CXX = g++
APP_NAME = Block_Puzzle
CPP_VERSION = 20

BIN_DIR = bin
OBJECT_DIR = $(BIN_DIR)/objects
INCLUDE_DIR = include
LIB_DIR = lib
SRC_DIR = src

CXX_FLAGS = -std=c++$(CPP_VERSION) -m64 -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/game -I$(INCLUDE_DIR)/$(APP_NAME)
SDL_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_gfx
LD_FLAGS = -L$(LIB_DIR) -lmingw32 $(SDL_FLAGS) -lbase64 -lH2DE

SRC = \
    $(wildcard $(SRC_DIR)/*.cpp) \
    $(wildcard $(SRC_DIR)/**/*.cpp) \
    $(wildcard $(SRC_DIR)/**/**/*.cpp)

OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(SRC))





all:
	make $(BIN_DIR)/$(APP_NAME).exe -j
	make run

$(BIN_DIR)/$(APP_NAME).exe: $(OBJ)
	$(CXX) $(CXX_FLAGS) -o $@ $^ $(LD_FLAGS)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXX_FLAGS) -c $< -o $@

run:
	cd $(BIN_DIR) && $(APP_NAME)

clean:
	@if exist "$(OBJECT_DIR)" rmdir /s /q "$(OBJECT_DIR)"

rebuild: clean all
