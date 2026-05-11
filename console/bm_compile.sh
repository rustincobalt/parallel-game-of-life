#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: ./bm_compile.sh <executable_name>"
    exit 1
fi

OUT=$1


# Path to raylib source or compiled library
PROJECT_HEADERS="../include/"

# Source file
CPP="./benchmarking.cpp \
    ../src/Life.cpp ../src/LifeSeq.cpp \
    ../src/LifeOMPBlocks.cpp \
    ../src/LifeOMPColumns.cpp \
    ../src/LifeOMPRows.cpp"

# Output executable
# OUT="benchmark_RTR.exe"

# Compile with g++
g++ -O2 -march=native $CPP -o "$OUT" \
    -I "$PROJECT_HEADERS" \
    -fopenmp

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation succeeded: $OUT"
else
    echo "Compilation failed."
fi
