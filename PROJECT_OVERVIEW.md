# C++ Todo List Application - Technical Overview

## Project Summary

This project is a full-stack Todo List application with a C++ backend and React frontend. It demonstrates modern C++ development practices, RESTful API design, and DevOps best practices for continuous integration and containerization.

## Technical Stack

### Backend
- **Language:** C++17
- **Build System:** CMake
- **Web Framework:** Drogon (with a simplified HTTP server for containerization)
- **Database:** SQLite3
- **JSON Handling:** JsonCPP
- **Testing:** Google Test

### Frontend
- **Framework:** React
- **Styling:** CSS
- **HTTP Client:** Axios

### DevOps
- **Containerization:** Docker
- **Orchestration:** Docker Compose
- **CI/CD:** GitHub Actions
- **Testing:** Automated unit and integration tests

## Architecture

### Core Components

1. **Task Model** (`Task.h`)
   - Defines the core data structure
   - Properties include ID, title, description, completion status, difficulty, and due date
   
2. **Todo List Manager** (`ToDoList.cpp/h`)
   - Manages task persistence
   - Handles CRUD operations for tasks
   - Integrates with SQLite database

3. **Task Prioritizer** (`TaskPrioritizer.cpp/h`)
   - Algorithm for prioritizing tasks based on difficulty and due dates
   - Implements custom sorting logic

4. **API Layer** (`api.cpp`)
   - RESTful API built with Drogon framework
   - Endpoints for task management, health checks, and data retrieval
   
5. **Simplified API** (`api_docker.cpp`)
   - Lightweight HTTP server implementation for containerization
   - No external dependencies (essential for Docker deployment)
   - Implements the same API contract as the full version

6. **React Frontend**
   - Modern, responsive user interface
   - State management for tasks
   - API integration with the C++ backend

## Design Choices & Implementation Details

### C++ Modern Features
The project leverages C++17 features including:
- `std::filesystem` for file operations
- Structured bindings
- String view optimizations
- Optional types

### Database Design
- SQLite was chosen for its simplicity and zero-configuration setup
- Database connection pooling in the Drogon version
- Schema designed for extensibility

### API Design
- RESTful principles
- Resources: `/api/tasks`, `/api/tasks/:id`, `/api/tasks/completed`, `/api/tasks/prioritized`
- HTTP methods: GET, POST, PUT, DELETE, OPTIONS
- CORS support for frontend integration
- Health endpoint for monitoring

### Build System
The CMake build system provides several options:
- `BUILD_TESTING`: Toggle for enabling/disabling tests
- `BUILD_API`: Toggle for building the main API with Drogon dependencies
- Automatic dependency resolution (FetchContent)
- Platform-specific configurations for macOS and Linux

### Testing Strategy
- Unit tests for core business logic using Google Test
- Integration tests for API endpoints
- Continuous integration via GitHub Actions

### DevOps Setup
1. **Docker Containerization**
   - Multi-stage builds for smaller image sizes
   - Non-root user for security
   - Health checks
   - Volume mounting for data persistence
   
2. **Docker Compose**
   - Service orchestration
   - Environment configuration
   - Networking between containers
   
3. **CI Pipeline**
   - Automated builds on push and pull requests
   - Dependency installation
   - Test execution
   - Docker image building

## Code Examples

### Task Structure
```cpp
struct Task {
    int id = -1;
    std::string header;
    std::string description;
    bool completed = false;
    int difficulty = 1;  // 1-5 scale
    std::string dueDate;
};
```

### Database Interaction
```cpp
bool ToDoList::connect(const std::string& dbPath) {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    // Create tasks table if it doesn't exist
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS tasks("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "header TEXT NOT NULL, "
        "description TEXT, "
        "completed INTEGER DEFAULT 0, "
        "difficulty INTEGER DEFAULT 1, "
        "due_date TEXT);";
        
    char* errMsg = nullptr;
    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}
```

### API Endpoint Implementation
```cpp
// Drogon API Handler
app().registerHandler("/api/tasks", 
    [&](const HttpRequestPtr& req, 
        std::function<void(const HttpResponsePtr&)>&& callback) {
        // Get all tasks
        auto resp = HttpResponse::newHttpJsonResponse(tasksToJson(todoList.getTasks()));
        resp->addHeader("Access-Control-Allow-Origin", "*");
        callback(resp);
    }, 
    {Get}
);
```

### Docker Configuration
```dockerfile
# Build stage
FROM ubuntu:22.04 as builder

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libsqlite3-dev \
    libjsoncpp-dev \
    git \
    uuid-dev \
    && rm -rf /var/lib/apt/lists/*

# Build application - use the simple API implementation only
RUN mkdir -p build && \
    cd build && \
    cmake -DBUILD_TESTING=OFF -DBUILD_API=OFF .. && \
    cmake --build . -j $(nproc)

# Runtime stage with only needed dependencies
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
    libsqlite3-0 \
    libjsoncpp25 \
    curl \
    && rm -rf /var/lib/apt/lists/*
```

## Performance Considerations

1. **Memory Management**
   - Careful use of reference parameters to avoid unnecessary copying
   - Smart pointers for resource management

2. **Database Optimizations**
   - Prepared statements for query performance
   - Appropriate indices on the tasks table
   - Transaction batching for bulk operations

3. **API Performance**
   - JSON response caching (in Drogon version)
   - Connection pooling
   - Asynchronous request handling

## Security Considerations

1. **Docker Security**
   - Non-root user in containers
   - Minimal dependencies in the runtime image
   - No hardcoded secrets

2. **API Security**
   - Input validation
   - Proper error handling
   - CORS configuration

## Future Enhancements

1. **Authentication**
   - User accounts
   - JWT authentication
   - Role-based permissions

2. **Additional Features**
   - Task categories/tags
   - Recurring tasks
   - Task sharing

3. **Infrastructure**
   - Kubernetes deployment
   - Database migration system
   - Monitoring and logging

## Conclusion

This project demonstrates a modern approach to C++ application development with a focus on maintainability, performance, and developer experience. The combination of a statically-typed, high-performance backend with a dynamic, responsive frontend creates a robust full-stack solution.

The DevOps setup ensures consistent building, testing, and deployment across environments, making the application production-ready and maintainable over time. 