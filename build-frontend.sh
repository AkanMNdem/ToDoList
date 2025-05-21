#!/bin/bash
# Script to build the frontend for production

# Define colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Building Todo List Frontend for Production ===${NC}"

# Navigate to frontend directory
cd "$(dirname "$0")/todo-list-frontend"

# Install dependencies if node_modules doesn't exist
if [ ! -d "node_modules" ]; then
    echo -e "${YELLOW}Installing dependencies...${NC}"
    npm install
fi

# Build the frontend
echo -e "${BLUE}Building production version...${NC}"
npm run build

# Check if build was successful
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Frontend build completed successfully!${NC}"
    echo -e "${BLUE}Production files are in: $(pwd)/build${NC}"
    
    # Optional: Copy to a serve directory or suggest how to serve
    echo -e "${YELLOW}To test the production build:${NC}"
    echo -e "  npm install -g serve"
    echo -e "  serve -s build"
else
    echo -e "${YELLOW}Frontend build failed.${NC}"
    exit 1
fi 