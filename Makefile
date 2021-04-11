SRC_DIR := src
LIB_DIR := lib
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/exe

LIB := $(wildcard $(LIB_DIR)/*.cpp)
OBJ := $(patsubst $(LIB_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(LIB)) # Convert .cpp files to .o

CXXFLAGS := -MMD -MP -Ilib
LDLIBS := -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: build dev clean

NAME := main

build: $(EXE)

dev: NAME := dev
dev: $(EXE)

$(EXE): $(OBJ) $(OBJ_DIR)/$(NAME).o | $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDLIBS)

$(OBJ_DIR)/$(NAME).o: $(SRC_DIR)/$(NAME).cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)