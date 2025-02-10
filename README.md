# ToDoList Application

## Overview
`ToDoList` is a simple task management application implemented in C++. It provides users with the ability to add, view, edit, and delete tasks, demonstrating fundamental concepts of data management, file handling, and user interaction in C++.

## Features
- **Add Tasks**: Create new tasks with titles and descriptions.
- **View Tasks**: Display all current tasks in an organized manner.
- **Edit Tasks**: Modify existing tasks' details.
- **Delete Tasks**: Remove tasks that are no longer needed.
- **Persistent Storage**: Tasks are saved between sessions using file handling.

## Technologies
- **Programming Language**: C++
- **Build System**: CMake for cross-platform builds
- **Containerization**: Docker support via `Dockerfile` and `docker-compose.yml`
- **Data Handling**: File-based storage for task persistence

## Getting Started

### Prerequisites
- **Environment**: Cross-platform (Windows, macOS, Linux)
- **Compiler**: Any C++ compiler supporting C++11 or later
- **Tools**: CMake, Docker (optional for containerized setup)

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/AkanMNdem/ToDoList.git
   ```

2. Navigate to the project directory:
   ```bash
   cd ToDoList
   ```

3. Build the project using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

### Using Docker (Optional)
1. Build and run the application using Docker Compose:
   ```bash
   docker-compose up --build
   ```

## Usage

### Running the Application
1. Start the ToDoList application:
   ```bash
   ./ToDoList
   ```
2. Follow the on-screen prompts to add, view, edit, or delete tasks.

## Code Structure
- **`src/`**: Contains the main source code files.
  - **`main.cpp`**: Entry point of the application.
  - **`task_manager.cpp`**: Core logic for managing tasks.

- **`include/`**: Header files for function declarations.
  - **`task_manager.h`**: Declares the task management functions and classes.

- **`data/`**: Directory used for storing task data persistently.
- **`CMakeLists.txt`**: Build configuration file for CMake.
- **`Dockerfile`**: Configuration for building a Docker container.
- **`docker-compose.yml`**: Configuration for running the application using Docker Compose.

## Functionality

### Task Management
1. **Add Tasks**: Users can create new tasks by providing titles and descriptions.
2. **View Tasks**: Lists all tasks with their details.
3. **Edit Tasks**: Allows modification of task titles and descriptions.
4. **Delete Tasks**: Removes tasks from the list and storage.

### Persistent Storage
- Tasks are saved in the `data/` directory, ensuring they persist between sessions.

## Future Enhancements
- **User Authentication**: Add user accounts to manage personal task lists.
- **Priority Levels**: Introduce task prioritization features.
- **Graphical User Interface**: Develop a GUI using frameworks like Qt or GTK.
- **Cloud Sync**: Enable task synchronization across devices via cloud storage.

## License
This project is licensed under the MIT License.

## Acknowledgments
This project serves as a foundational exercise in C++ programming, emphasizing task management, file handling, and modular design principles.

