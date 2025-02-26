# Requirements Document for ToDoList Application

## I. MVP Requirements (Basic Application)

### Functional Requirements
- **Task Creation**
  - Ability to add a new task with:
    - Title (string)
    - Description (string)
    - Optional due date (date format, e.g., YYYY-MM-DD)
    - Difficulty level (integer 1–5; used in prioritization algorithm)
- **Task Management**
  - **View Tasks:** Display tasks in a prioritized order based on both due date and difficulty.
    - The prioritization algorithm should be configurable and based on either established research or custom rules that order tasks (e.g., from easy to hard or as supported by academic findings).
  - **Edit Tasks:** Update a task’s title, description, due date, and difficulty level.
    - Note: Complex task reordering due to the prioritization algorithm might require additional logic when a task is updated or marked as completed.
  - **Delete Tasks:** Remove a task from the list and persistent storage.
- **Task Updates & Refresh**
  - The task list should refresh:
    - Immediately whenever a new task is added.
    - Automatically once each day at 9:00 AM to reapply the prioritization algorithm and update the order.
- **Persistent Storage**
  - Use a database (SQLite) for storing tasks rather than file-based storage.
  - Ensure efficient queries and indexing to support dynamic sorting and filtering.
- **Backend Prioritization Engine**
  - Develop a module that calculates task priority using both due dates and difficulty levels.
  - This module should be designed to incorporate insights from academic research on optimal task ordering.
  - Allow for future adjustments or configuration changes in the algorithm without major refactoring.

### Non-Functional Requirements
- **Platform Compatibility & Scalability**
  - The backend should be designed in C++ (using C++11 or later) and be modular to facilitate future scaling.
  - Ensure compatibility with Windows, macOS, and Linux environments.
- **Modularity & Maintainability**
  - Organize code into distinct modules:
    - Task management (CRUD operations and prioritization logic)
    - Database handling (SQLite interactions)
    - Web server backend (API endpoints for the frontend)
    - Frontend integration (webGUI components)
- **Performance**
  - Optimize database queries and prioritization algorithms to handle an increasing number of tasks without significant delays.
- **Build & Deployment**
  - Utilize CMake for building the C++ backend.
  - Support containerized deployment (e.g., via Docker) for private use on self-hosted servers or cloud environments.
- **Security & Privacy**
  - Ensure that data stored in SQLite and communicated between the backend and webGUI is secure.
  - Implement access controls for the private deployment environment.

---

## II. Future Enhancements (Iterative Updates)

### Advanced Functional Requirements
- **Enhanced Prioritization and Scheduling**
  - Refine the task prioritization engine by incorporating additional academic research on task scheduling and ordering.
  - Allow users to select or adjust the prioritization criteria (e.g., weighting due dates vs. difficulty).
- **Multiple Task Lists & Categories**
  - Enable users to create and manage multiple task lists based on categories (e.g., Work, Personal, Urgent).
  - Provide an interface to switch between or merge lists as needed.
- **WebGUI Frontend**
  - Develop a responsive web-based graphical user interface (webGUI) that communicates with the backend via RESTful APIs.
  - The webGUI should display tasks along with a header section summarizing high-priority tasks (determined by the prioritization algorithm).
- **Auto-Rescheduling / Time Allocation**
  - In cases where the estimated workload (based on difficulty) exceeds available daily time:
    - Offer an option to automatically adjust due dates using a default time allocation strategy.
    - Provide a bulk-update feature to reschedule multiple tasks simultaneously.
- **Deployment Enhancements**
  - Expand deployment options to include support for private cloud environments or on-premises servers.
  - Implement automated updates and backups for the SQLite database.
- **User Customization & Settings**
  - Allow users to customize how the prioritization algorithm calculates task order.
  - Enable configuration settings for auto-refresh frequency, notification thresholds, and other behavioral parameters.

---

## III. Methodology & Iterative Development

- **Agile Development Approach**
  - Break the project into iterative sprints:
    - Start with the core task management MVP (using SQLite for persistence and a basic prioritization algorithm).
    - Gradually integrate the webGUI, advanced scheduling, and deployment enhancements.
- **Documentation & Testing**
  - Maintain comprehensive documentation of both requirements and code.
  - Develop unit tests and integration tests, especially for the prioritization engine and database interactions.
- **User Feedback**
  - Collect and analyze user feedback on the MVP to refine the prioritization logic and user interface in subsequent releases.