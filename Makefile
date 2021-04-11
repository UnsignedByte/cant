SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

MAIN := $(BIN_DIR)/main #output exe

SRC := $(wildcard $(SRC_DIR)/*.cc)
OBJ := $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(SRC)) # Convert .cc files to .o

CXXFLAGS := -Iinclude
LDLIBS := -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: all clean

all: $(MAIN)

$(MAIN): $(OBJ) | $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDLIBS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)