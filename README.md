# ToDoList Application

## Overview
A modern task management application with a C++ backend and React frontend. It allows users to create, view, prioritize, edit, and delete tasks with an intelligent prioritization algorithm.

## Features
- **Task Management**: Create, view, edit, and delete tasks
- **Task Prioritization**: Intelligent algorithm orders tasks based on due dates and difficulty
- **Completion Tracking**: Mark tasks as completed or uncompleted
- **Multiple Views**: View all tasks, prioritized tasks, or completed tasks
- **Persistent Storage**: SQLite database ensures tasks persist between sessions

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

## Getting Started

### Prerequisites
- C++ compiler supporting C++17
- CMake 3.15 or higher
- Node.js and npm
- SQLite3

### Backend Setup
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

### Frontend Setup
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
| GET | /api/prioritization/strategies | List available prioritization strategies |

## Project Structure
- **`src/`**: Backend C++ source files
  - **`main.cpp`**: Console application entry point
  - **`api.cpp`**: API server implementation with Drogon
  - **`ToDoList.cpp`**: Core task management logic
  - **`TaskPrioritizer.cpp`**: Task prioritization algorithms

- **`include/`**: Header files
  - **`Task.h`**: Task data structure
  - **`ToDoList.h`**: Task management interface
  - **`TaskPrioritizer.h`**: Prioritization algorithm declarations

- **`todo-list-frontend/`**: React frontend application
  - **`src/components/`**: React components
  - **`src/services/`**: API services and utilities

- **`data/`**: Contains the SQLite database
- **`CMakeLists.txt`**: Build configuration file
- **`build.sh`**: Build script for the C++ backend

## Future Enhancements
- Daily automatic task refresh at 9:00 AM
- Enhanced prioritization algorithms based on research
- User authentication and multiple user support
- Mobile application
- Dark mode and additional UI themes

## License
This project is licensed under the MIT License.