#!/bin/bash

# Path to raylib source or compiled library
PROJECT_HEADERS="include/"
RAYLIB_HEADER="lib/"
RAYLIB_LIB="lib/"

# Source file
CPP="src/main.cpp \
    src/App.cpp src/AppState.cpp src/AppUI.cpp \
    src/raygui_impl.cpp \
    src/Life.cpp src/LifeSeq.cpp \
    src/LifeOMPBlocks.cpp \
    src/LifeOMPColumns.cpp \
    src/LifeOMPRows.cpp"

# Output executable
OUT="game_of_life.exe"

# Compile with g++
g++ -O3 -march=native $CPP -o "$OUT" \
    -I "$PROJECT_HEADERS" -I "$RAYLIB_HEADER" \
    -L "$RAYLIB_LIB" \
    -fopenmp \
    -lraylib -lopengl32 -lgdi32 -lwinmm \
    -D__WINDOWS_MM__ \
    -static -static-libgcc -static-libstdc++

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation succeeded: $OUT"
else
    echo "Compilation failed."
fi
