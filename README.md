# ToDoList Application

## Overview
A modern task management application with a C++ backend and React frontend. It allows users to create, view, prioritize, edit, and delete tasks with an intelligent prioritization algorithm.

## Features
- **Task Management**: Create, view, edit, and delete tasks
- **Task Prioritization**: Intelligent algorithm orders tasks based on due dates and difficulty
- **Completion Tracking**: Mark tasks as completed or uncompleted
- **Multiple Views**: View all tasks, prioritized tasks, or completed tasks
- **Persistent Storage**: SQLite database ensures tasks persist between sessions
- **DevOps Ready**: Docker containerization, CI/CD pipeline, and automated testing

## Technologies
### Backend
- **Programming Language**: C++ (C++17)
- **Web Framework**: Drogon - high-performance C++ web framework
- **Database**: SQLite for efficient local data storage
- **Build System**: CMake for cross-platform builds

### Frontend
- **Framework**: React with functional components and hooks
- **HTTP Client**: Axios for API communication
- **Styling**: Inline CSS with React (expandable to CSS frameworks)

### DevOps and Testing
- **Containerization**: Docker with multi-stage builds
- **Orchestration**: Docker Compose for service coordination
- **CI/CD**: GitHub Actions for automated builds and tests
- **Unit Testing**: Google Test for C++ unit tests
- **API Testing**: Integration tests with curl/libcurl

## Getting Started

### Prerequisites
- C++ compiler supporting C++17
- CMake 3.15 or higher
- Node.js and npm
- SQLite3
- Docker and Docker Compose (optional for containerized setup)

### Method 1: Using Development Scripts

We provide several scripts to simplify the development workflow:

1. **Start local development environment**:
   ```bash
   ./dev.sh
   ```
   This script builds the project if needed, starts the backend API server, and launches the React development server.

2. **Start Docker-based development environment**:
   ```bash
   ./docker-dev.sh
   ```
   This script builds and starts both the backend and frontend containers using Docker Compose.

3. **Build frontend for production**:
   ```bash
   ./build-frontend.sh
   ```
   This script builds an optimized production version of the frontend.

### Method 2: Direct Setup

#### Backend Setup
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/to-do-list-cpp.git
   cd to-do-list-cpp
   ```

2. Build the C++ backend:
   ```bash
   ./build.sh
   ```

3. Run the API server:
   ```bash
   ./build/todo_api
   ```
   The API server will start on http://localhost:8080

#### Frontend Setup
1. Navigate to the frontend directory:
   ```bash
   cd todo-list-frontend
   ```

2. Install dependencies:
   ```bash
   npm install
   ```

3. Start the development server:
   ```bash
   npm start
   ```
   The frontend will be available at http://localhost:3000

### Method 2: Docker Setup
1. Clone the repository and navigate to the project directory:
   ```bash
   git clone https://github.com/yourusername/to-do-list-cpp.git
   cd to-do-list-cpp
   ```

2. Build and start the containers:
   ```bash
   docker-compose up -d
   ```

3. Access the application:
   - Backend API: http://localhost:8080
   - Frontend: http://localhost:3000

## API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | /api/tasks | List all tasks |
| GET | /api/tasks/{id} | Get a specific task |
| POST | /api/tasks | Create a new task |
| PUT | /api/tasks/{id} | Update a task |
| DELETE | /api/tasks/{id} | Delete a task |
| POST | /api/tasks/{id}/complete | Mark a task as completed |
| POST | /api/tasks/{id}/uncomplete | Mark a task as uncompleted |
| GET | /api/tasks/completed | List completed tasks |
| GET | /api/tasks/prioritized | List tasks in prioritized order |
| GET | /health | Health check endpoint for monitoring |

## Project Structure
- **`src/`**: Backend C++ source files
  - **`main.cpp`**: Console application entry point
  - **`api.cpp`**: API server implementation with Drogon
  - **`api_docker.cpp`**: Simplified API server for Docker
  - **`ToDoList.cpp`**: Core task management logic
  - **`TaskPrioritizer.cpp`**: Task prioritization algorithms

- **`include/`**: Header files
  - **`Task.h`**: Task data structure
  - **`ToDoList.h`**: Task management interface
  - **`TaskPrioritizer.h`**: Prioritization algorithm declarations

- **`tests/`**: Testing directory
  - **`core/`**: Unit tests for core functionality
  - **`api/`**: API integration tests

- **`todo-list-frontend/`**: React frontend application
  - **`src/components/`**: React components
  - **`src/services/`**: API services and utilities

- **`.github/workflows/`**: CI/CD pipeline configuration
- **`data/`**: Contains the SQLite database
- **`CMakeLists.txt`**: Build configuration file
- **`build.sh`**: Build script for the C++ backend
- **`Dockerfile`**: Container definition for the backend
- **`docker-compose.yml`**: Multi-container orchestration config

## Testing
Run tests using the following commands:

```bash
# Build with tests enabled
mkdir -p build && cd build
cmake ..
make

# Run all tests
ctest --verbose

# Or run the test target directly
make run_tests
```

## Development Workflow
1. Create a feature branch from `develop`
2. Make your changes
3. Run tests to verify functionality
4. Create a pull request to merge back to `develop`
5. CI pipeline will automatically build and test your changes
6. After approval, changes are merged

## Future Enhancements
- User authentication and multiple user support
- Enhanced prioritization algorithms based on research
- Mobile application
- Dark mode and additional UI themes
- Kubernetes deployment for production environments
- Task categories and tagging system
- Consistent design system with reusable components
- React Router implementation for proper page navigation
- Breadcrumbs for better user orientation
- Transitions between pages for smoother experience
- Enhanced accessibility features
- Advanced search and filtering capabilities
- Responsive design optimizations for all devices

## License
This project is licensed under the MIT License.