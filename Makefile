# Graphics Engine Makefile
CC = gcc
CFLAGS = -O3 -march=native -mtune=native -mavx2 -mfma \
         -ffast-math -fno-trapping-math -fno-math-errno \
         -fstrict-aliasing -fopenmp \
         -D PROCESS_GUARD=1 -I.
LDFLAGS = -L/mingw64/lib -lglfw3 -lglew32 -lopengl32 -lm -fopenmp 
#   -std=c99 -static, -Wall -Wextra
# Directories
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/engine

# Source files with full paths
SOURCES = main.c \
          0_engine/0_engine/engine.c \
          0_engine/1_renderer/renderer.c \
          0_engine/2_scene/scene.c \
          0_engine/3_camera/camera.c \
          0_engine/4_input/input.c \
          0_engine/5_math/math3d.c

# Object files (flatten to obj/ directory)
OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SOURCES)))

# Default target
all: clean build run

# Build only
build: $(TARGET)
# 	@echo "Build complete: $(TARGET)"

# Link executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
# 	@echo "Linked: $(TARGET)"

# Compile main.c
$(OBJ_DIR)/main.o: main.c
	@mkdir -p $(OBJ_DIR)

	$(CC) $(CFLAGS) -c $< -o $@

# Compile engine.c
$(OBJ_DIR)/engine.o: 0_engine/0_engine/engine.c
	@mkdir -p $(OBJ_DIR)

	$(CC) $(CFLAGS) -c $< -o $@

# Compile renderer.c
$(OBJ_DIR)/renderer.o: 0_engine/1_renderer/renderer.c
	@mkdir -p $(OBJ_DIR)

	$(CC) $(CFLAGS) -c $< -o $@

# Compile scene.c
$(OBJ_DIR)/scene.o: 0_engine/2_scene/scene.c
	@mkdir -p $(OBJ_DIR)

	$(CC) $(CFLAGS) -c $< -o $@

# Compile camera.c
$(OBJ_DIR)/camera.o: 0_engine/3_camera/camera.c
	@mkdir -p $(OBJ_DIR)

	$(CC) $(CFLAGS) -c $< -o $@

# Compile input.c
$(OBJ_DIR)/input.o: 0_engine/4_input/input.c
	@mkdir -p $(OBJ_DIR)

	$(CC) $(CFLAGS) -c $< -o $@

# Compile math3d.c
$(OBJ_DIR)/math3d.o: 0_engine/5_math/math3d.c
	@mkdir -p $(OBJ_DIR)

	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning previous build..."
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN_DIR)
	@echo "Clean complete"

# Rebuild from scratch and run
rebuild: clean build run

# Run the program
run: $(TARGET)
	@echo "Running engine..."
	@./$(TARGET)

.PHONY: all clean rebuild run build