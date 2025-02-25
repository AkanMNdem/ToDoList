# Requirements Document for ToDoList Application

## I. MVP Requirements (Basic Application)

### Functional Requirements
- **Task Creation**
  - Ability to add a new task with:
    - Title (string)
    - Description (string)
    - Optional due date (date format, e.g., YYYY-MM-DD)
    - Difficulty level (integer 1–5; store value for future time conversion)
- **Task Management**
  - **View Tasks:** List all tasks with details in a clear, organized format.
  - **Edit Tasks:** Update a task’s title, description, due date, and difficulty level.
  - **Delete Tasks:** Remove a task from the list and persistent storage.
- **Task Sorting & Filtering**
  - Sort tasks by due date.
  - Sort tasks by difficulty level.
  - (Optional) Filter tasks by completion status if a “completed” flag is added.
- **Persistent Storage**
  - Save tasks to a file in the `data/` directory.
  - Load tasks from the file when the application starts.
- **User Interface (CLI)**
  - Present a clear menu for task operations (add, view, edit, delete).
  - Display confirmation messages upon successful task creation, modification, or deletion.

### Non-Functional Requirements
- **Platform Compatibility**
  - Must build and run on Windows, macOS, and Linux.
- **Modularity & Maintainability**
  - Code must be organized into modules:
    - Task management (logic for task CRUD operations)
    - File handling (persistent storage)
    - User interface (CLI handling)
  - Use C++11 or later standards for improved clarity and performance.
- **Performance**
  - Ensure that operations like sorting and filtering are efficient, even with a growing number of tasks.
- **Build & Deployment**
  - Utilize CMake for cross-platform builds.
  - Support Docker for containerized deployment.

---

## II. Future Enhancements (Iterative Updates)

### Advanced Functional Requirements
- **Enhanced Task Prioritization**
  - Implement a conversion mechanism that maps difficulty levels (1–5) to time estimates.
  - Develop a prioritization algorithm that considers both due dates and difficulty levels to identify the most pressing tasks.
- **Multiple Task Lists & Categories**
  - Allow users to create and manage multiple task lists based on categories (e.g., Work, Personal, Urgent).
  - Provide a navigation system to switch between different task lists.
- **Dashboard/Header View**
  - Design a header section on the main page that displays a summary of high-priority tasks.
  - Dynamically update the header to show tasks nearing their due dates or with high difficulty.
- **Auto-Rescheduling / Time Allocation**
  - If total estimated time exceeds available time in the day:
    - Provide an option to auto-adjust due dates based on a default time allocation.
    - Offer a bulk-update feature to modify due dates for all tasks simultaneously.
- **User Customization**
  - Allow users to configure the mapping between difficulty levels and time estimates.
  - Enable customization of sorting and filtering preferences.

### Additional Enhancements & Features
- **Graphical User Interface (GUI)**
  - Develop a GUI using frameworks like Qt or GTK for a more intuitive experience.
- **Cloud Synchronization**
  - Integrate cloud storage options or synchronization with services (e.g., Google Tasks, Dropbox) for multi-device access.
- **User Authentication**
  - Implement user account management to allow personalized task lists.
- **Notifications & Reminders**
  - Add a notification system to alert users of upcoming due dates or when tasks require immediate attention.

---

## III. Methodology & Iterative Development

- **Agile Development Approach**
  - Break the project into iterative sprints:
    - Start with the core task management MVP.
    - Gradually implement advanced features (prioritization, multiple lists, auto-rescheduling).
- **Documentation & Testing**
  - Keep the requirements document and code documentation up-to-date.
  - Develop unit tests and integration tests to ensure stability as new features are added.
- **User Feedback**
  - Collect and analyze user feedback on the MVP to inform subsequent feature developments and refinements.