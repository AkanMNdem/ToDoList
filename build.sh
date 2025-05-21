#!/bin/bash

set -e  # Exit on first error
BUILD_DIR="build"

# Clean previous build if exists
if [ "$1" == "clean" ]; then
  echo "🧹 Cleaning build directory..."
  rm -rf $BUILD_DIR
  exit 0
fi

# Configure
echo "⚙️  Configuring project with Ninja..."
cmake -G Ninja -B $BUILD_DIR -S .

# Build
echo "🔨 Building project with Ninja..."
ninja -C $BUILD_DIR

# Run (if applicable)
echo "🚀 Running todo_api (if build succeeded)..."
$BUILD_DIR/todo_api