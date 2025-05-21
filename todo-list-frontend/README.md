# Todo List Frontend

A React-based frontend for the Todo List application that communicates with a C++ Drogon backend API.

## Features

- View all tasks in a clean, organized interface
- Add new tasks with titles, descriptions, difficulty levels, and due dates
- Mark tasks as completed or uncompleted
- Delete tasks
- View tasks by different categories (All, Prioritized, Completed)

## Technologies Used

- **React**: Frontend library for building the user interface
- **Axios**: HTTP client for API communication
- **Create React App**: Project scaffolding and build tooling

## Project Structure

- **`src/components/`**: React components
  - **`TaskForm.jsx`**: Form for creating new tasks
  - **`TaskItem.jsx`**: Individual task display component
  - **`TaskList.jsx`**: Component for displaying lists of tasks
- **`src/services/`**: API communication
  - **`taskService.js`**: Axios-based API client for the backend

## Getting Started

### Prerequisites

- Node.js and npm installed
- Backend API server running (see main project README)

### Installation

1. Install dependencies:
   ```bash
   npm install
   ```

2. Start the development server:
   ```bash
   npm start
   ```

3. Build for production:
   ```bash
   npm run build
   ```

## Using the Application

1. **Adding Tasks**: Fill out the form at the top of the page and click "Add Task"
2. **Viewing Tasks**: Tasks appear in the list below the form
3. **Task Filtering**: Use the buttons above the task list to filter by All, Prioritized, or Completed
4. **Managing Tasks**: Use the buttons on each task to mark as complete/incomplete or delete

## Connecting to the Backend

The frontend connects to the C++ backend API at `http://localhost:8080/api`. If your backend is running on a different URL, update the `API_URL` constant in `src/services/taskService.js`.

## Customization

- Modify styles in component files or create dedicated CSS files
- Add new components in the `src/components` directory
- Extend API functionality in `src/services/taskService.js`

## Deployment

For production deployment:
1. Build the frontend: `npm run build`
2. Serve the static files in the `build` directory using Nginx, Apache, or a static hosting service

## Future Enhancements

- Task editing functionality
- More detailed task views
- User authentication
- Additional filtering and sorting options
- Dark mode toggle