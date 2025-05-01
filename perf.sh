#!/bin/bash

set -e

COMPILER="./dist/1cc/compiler"

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
  --compiler)
    COMPILER="$2"
    shift 2
    ;;
  *)
    echo "Unknown option: $1"
    echo "Usage: ./perf.sh --compiler <compiler_path>"
    exit 1
    ;;
  esac
done

# Ensure hyperfine is installed
if ! command -v hyperfine &>/dev/null; then
  echo "Error: hyperfine is not installed"
  echo "Please install it first: https://github.com/sharkdp/hyperfine"
  exit 1
fi

# Ensure the compiler exists
if ! command -v "$COMPILER" &>/dev/null; then
  echo "Error: Compiler '$COMPILER' not found"
  exit 1
fi

# Create a temporary build directory
BUILD_DIR=$(mktemp -d)
trap 'rm -rf "$BUILD_DIR"' EXIT

export BUILD_DIR
export COMPILER

# Run the performance test with hyperfine
hyperfine --warmup 2 \
  --prepare "rm -rf $BUILD_DIR/* && mkdir -p $BUILD_DIR" \
  'for file in src/*.c; do
    base_filename=$(basename "$file")
    "$COMPILER" --output "$BUILD_DIR/${base_filename%.c}.ll" "$file"
  done'
