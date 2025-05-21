#!/bin/bash
# Docker-based development script

# Define colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Starting Todo List Docker Development Environment ===${NC}"

# Check if Docker is running
if ! docker info > /dev/null 2>&1; then
    echo -e "${YELLOW}Docker is not running. Please start Docker and try again.${NC}"
    exit 1
fi

# Build and start the containers
echo -e "${BLUE}Building and starting Docker containers...${NC}"
docker-compose up --build

# This part will execute when docker-compose is terminated
echo -e "${YELLOW}Shutting down Docker containers...${NC}"
docker-compose down 