# Tech Stack Document for ToDoList Application

## Overview
This document outlines the technology stack for the ToDoList Application. The application is structured into three main layers:
- **Backend:** Implements business logic, RESTful APIs, and data persistence using C++ and SQLite.
- **Frontend:** Provides a responsive web-based GUI for task management.
- **Deployment & Management:** Uses containerization and CI/CD to ensure scalable and maintainable deployments.

---

## Backend

**Programming Language:**  
- **C++ (C++17)**
  - **Why?**  
    - High performance and efficient memory management.
    - Fine-grained control over system resources.
    - Already part of the current project foundation.

**Web Framework:**  
- **Drogon** (implemented)
  - **Why?**  
    - Modern C++ web framework with excellent performance.
    - Built-in JSON support and asynchronous request handling.
    - Clear path parameter handling and middleware support.
    - No dependency on deprecated Boost APIs.

**Build System:**  
- **CMake** (implemented)
  - **Why?**  
    - Enables cross-platform builds.
    - Manages project dependencies and simplifies compilation across different environments.

**Database:**  
- **SQLite** (implemented)
  - **Why?**  
    - Simple, file-based relational database.
    - Ideal for MVP and scalable enough for small-to-medium workloads.
    - Low overhead and minimal configuration.

**Logging:**  
- **Built-in console logging** (implemented)
  - **Why?**  
    - Simplified approach for MVP.
    - Future enhancement: Integrate with spdlog or similar.

**Testing Framework:**  
- **Manual testing with curl** (implemented)
  - Future plans to add automated tests with Google Test or Catch2

---

## Frontend

**Framework:**  
- **React** (implemented)
  - **Why?**  
    - Popular component-based library with extensive ecosystem.
    - Efficient rendering through virtual DOM.
    - Supports functional components and hooks for state management.

**Routing & Navigation:**
- **React Router** (planned)
  - **Why?**
    - Industry standard for handling routing in React applications
    - Enables multi-page experience with client-side routing
    - Supports nested routes, route parameters, and navigation guards

**Languages & Technologies:**  
- **HTML, CSS, and JavaScript (ES6+)** (implemented)
  - **Why?**  
    - Standard technologies for building web interfaces.
    - Ensure broad browser compatibility and a responsive design.

**UI Enhancement:**
- **CSS Variables** (implemented)
  - **Why?**
    - Enables theming and consistent styling across components
    - Simplifies theme switching functionality
- **Responsive Design** (planned enhancement)
  - **Why?**
    - Ensures optimal user experience across different device sizes
    - Improves accessibility and usability

**Tooling:**  
- **Bundler:** Create React App (implemented)
  - **Why?**  
    - Quickly bootstraps a React application with good defaults.
    - Handles webpack configuration and build process.
- **Package Manager:** npm (implemented)
  - **Why?**  
    - Manages dependencies and simplifies script execution.
- **HTTP Client:** Axios (implemented)
  - **Why?**
    - Promise-based HTTP client with easy-to-use API.
    - Automatic JSON transformation and error handling.

**State Management:**
- **React Context API** (implemented for theming)
  - **Why?**
    - Built-in solution for managing global state
    - Avoids prop drilling in component hierarchies
    - Suitable for medium-sized applications

---

## Deployment & Management

**Containerization:**  
- **Docker** (planned)
  - **Why?**  
    - Provides isolated, reproducible environments.
    - Simplifies the deployment process across different platforms.

**CI/CD Pipeline:**  
- **GitHub Actions** (planned)
  - **Why?**  
    - Automates building, testing, and deployment processes.
    - Integrates well with GitHub repositories.

**Management Scripts:**  
- **build.sh for backend** (implemented)
  - **Why?**  
    - Simplifies building the C++ application.
    - Future plans to extend for deployment scenarios.

---

## Current Implementation Status

- **Backend:** 100% implemented (C++ with Drogon, SQLite)
- **Frontend:** Basic implementation complete (React)
- **Deployment:** Local development setup only
- **API Integration:** Functional with proper error handling

## Future Tech Considerations

- Add proper CORS handling for production
- Implement user authentication system
- Add automated testing for both backend and frontend
- Containerize the application with Docker