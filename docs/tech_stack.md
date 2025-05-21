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
- **Crow or Wt**
- **Drogon** --> final choice due to Boost API dependency
  - **Why?**  
    - Modern C++ web framework with excellent performance.
    - Built-in JSON support and asynchronous request handling.
    - Clear path parameter handling and middleware support.
    - No dependency on deprecated Boost APIs.

**Build System:**  
- **CMake**
  - **Why?**  
    - Enables cross-platform builds.
    - Manages project dependencies and simplifies compilation across different environments.

**Database:**  
- **SQLite**
  - **Why?**  
    - Simple, file-based relational database.
    - Ideal for MVP and scalable enough for small-to-medium workloads.
    - Low overhead and minimal configuration.

**Logging:**  
- **Built-in console logging**
  - **Why?**  
    - Simplified approach for MVP.
    - Future enhancement: Integrate with spdlog or similar.

**Testing Framework:**  
- **Google Test or Catch2** (planned)
  - **Why?**  
    - Supports unit and integration testing.
    - Ensures backend logic and API endpoints remain robust during development.

---

## Frontend

**Framework:**  
- **React, Vue.js, or Angular**
  - **Why?**  
    - These modern JavaScript frameworks allow you to build dynamic single-page applications (SPAs).
    - Support component-based architecture, which improves maintainability and scalability.

**Languages & Technologies:**  
- **HTML, CSS, and JavaScript (ES6+)**
  - **Why?**  
    - Standard technologies for building web interfaces.
    - Ensure broad browser compatibility and a responsive design.

**Tooling:**  
- **Bundler:** Webpack or Create React App (if using React)
  - **Why?**  
    - Efficiently bundles and optimizes your assets for production.
- **Package Manager:** npm or Yarn
  - **Why?**  
    - Manages dependencies and simplifies script execution.
- **UI Libraries (Optional):** Bootstrap, Material-UI, or Tailwind CSS
  - **Why?**  
    - Accelerates development of a consistent and responsive design.

---

## Deployment & Management

**Containerization:**  
- **Docker**
  - **Why?**  
    - Provides isolated, reproducible environments.
    - Simplifies the deployment process across different platforms (development, testing, production).

**Orchestration:**  
- **Docker Compose**
  - **Why?**  
    - Manages multi-container setups.
    - Simplifies the communication between the backend, frontend, and other services like monitoring or logging.

**CI/CD Pipeline:**  
- **GitHub Actions, Travis CI, or Jenkins**
  - **Why?**  
    - Automates building, testing, and deployment processes.
    - Ensures consistent and reliable application updates.

**Management Scripts:**  
- **Database Migration & Backup Scripts**
  - **Why?**  
    - Facilitate smooth schema updates and data integrity.
- **Orchestration & Log Rotation Scripts**
  - **Why?**  
    - Help in maintaining, monitoring, and managing containers efficiently.

**Monitoring & Logging:**  
- **Monitoring Tools:** Prometheus, Grafana, or basic health-check endpoints
  - **Why?**  
    - Track application performance and uptime.
- **Centralized Logging (Optional):** ELK Stack (Elasticsearch, Logstash, Kibana)
  - **Why?**  
    - Aggregates logs for better insights and troubleshooting in production.

---

## Summary

This tech stack is designed to support a scalable, maintainable, and high-performance ToDoList Application:

- **Backend:**  
  C++ with a lightweight web framework for efficiency, coupled with SQLite for simple yet robust data storage.

- **Frontend:**  
  A modern SPA built with React/Vue.js/Angular ensures a rich user experience.

- **Deployment:**  
  Docker and Docker Compose streamline the deployment process, while CI/CD and management scripts help maintain and scale the application reliably.

This comprehensive approach will not only meet the current MVP requirements but also provide a solid foundation for future enhancements and scalability.