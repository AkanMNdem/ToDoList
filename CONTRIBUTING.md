# Contributing to ToDoList App

Thank you for considering contributing to our project! Here's how to get started.

## Development Environment Setup

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/to-do-list-cpp.git
   cd to-do-list-cpp
   ```

2. **Build the backend**
   ```bash
   ./build.sh
   ```

3. **Set up the frontend**
   ```bash
   cd todo-list-frontend
   npm install
   npm start
   ```

4. **Run with Docker (alternative)**
   ```bash
   docker-compose up
   ```

## Development Workflow

1. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes**
   - Follow the code style of the project
   - Add tests for your changes
   - Update documentation if necessary

3. **Run tests**
   ```bash
   # For backend tests
   cd build && make run_tests
   
   # For frontend tests
   cd todo-list-frontend && npm test
   ```

4. **Commit your changes**
   ```bash
   git add .
   git commit -m "Brief description of your changes"
   ```

5. **Push your branch**
   ```bash
   git push -u origin feature/your-feature-name
   ```

6. **Open a Pull Request**
   - Use the PR template
   - Address any feedback from code reviews

## Coding Standards

- **C++ Code**
  - Follow modern C++17 practices
  - Use proper error handling
  - Document complex functions
  - Write unit tests for new functionality

- **React Code**
  - Follow functional component patterns
  - Use TypeScript for type safety
  - Follow React hooks best practices
  - Write component tests

## Project Structure

- `/src` - C++ backend source code
- `/include` - C++ header files
- `/tests` - C++ tests
- `/todo-list-frontend` - React frontend
- `/docs` - Documentation

## Getting Help

If you have questions, feel free to open an issue or contact the maintainers. 