#!/bin/bash

# Path to raylib source or compiled library
PROJECT_HEADERS="include/"
RAYLIB_HEADER="lib/"
RAYLIB_LIB="lib/"

# Source file
CPP="src/main.cpp src/App.cpp src/AppUI.cpp src/raygui_impl.cpp"

# Output executable
OUT="game of life.exe"

# Compile with g++
g++ $CPP -o "$OUT" -I "$PROJECT_HEADERS" -I "$RAYLIB_HEADER" -L "$RAYLIB_LIB" -lraylib -lopengl32 -lgdi32 -lwinmm -D__WINDOWS_MM__

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation succeeded: $OUT"
else
    echo "Compilation failed."
fi
