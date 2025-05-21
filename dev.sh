#!/bin/bash
# Development script to start both backend and frontend

# Define colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Clear the terminal before starting
clear

echo -e "${GREEN}=== Starting Todo List Development Environment ===${NC}"

# Check if the build directory exists and the todo_api binary is present
if [ ! -f "./build/todo_api" ]; then
    echo -e "${YELLOW}Backend API binary not found. Building the project...${NC}"
    ./build.sh
fi

# Start the backend API server in the background
echo -e "${GREEN}Starting backend API server...${NC}"
./build/todo_api &
BACKEND_PID=$!

# Save the PID to a file for later cleanup
echo $BACKEND_PID > .backend_pid

# Give the backend a moment to start
sleep 2

# Print information about the servers
echo -e "${BLUE}Backend API is running at:${NC} http://localhost:8080"
echo -e "${BLUE}Frontend will be available at:${NC} http://localhost:3000"
echo -e "${YELLOW}Note: Please manually open http://localhost:3000 in your browser if it doesn't open automatically${NC}"

# Start the frontend development server without opening browser automatically
echo -e "${GREEN}Starting frontend development server...${NC}"
cd todo-list-frontend
BROWSER=none npm start

# Cleanup function to kill backend when script exits
cleanup() {
    echo -e "${YELLOW}Shutting down servers...${NC}"
    if [ -f "../.backend_pid" ]; then
        PID=$(cat ../.backend_pid)
        kill -9 $PID 2>/dev/null
        rm ../.backend_pid
    fi
    exit 0
}

# Register the cleanup function for when the script exits
trap cleanup EXIT

# This will not execute unless npm start fails or is manually terminated
cleanup 