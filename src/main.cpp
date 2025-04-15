#include "TaskPrioritizer.h"
#include "ToDoList.h"
#include <filesystem>
#include <iostream>

int main() {
  try {
    // Check if data directory exists, create if not
    std::filesystem::path dataDir = "data";
    if (!std::filesystem::exists(dataDir)) {
      std::filesystem::create_directory(dataDir);
    }

    // Connect to database
    ToDoList todoList;
    todoList.connect("data/tasks.db");
    std::cout << "Connected to database successfully.\n";

    // Basic task management demo
    std::cout << "\n===== ToDo List Application =====\n";

    // Add sample tasks if database is empty
    auto tasks = todoList.getTasks();
    if (tasks.empty()) {
      std::cout << "Adding sample tasks...\n";
      todoList.addTask("Complete C++ backend",
                       "Implement ToDoList class functionality", 3,
                       "2023-12-15");
      todoList.addTask("Write tests",
                       "Create unit tests for basic functionality", 2,
                       "2023-12-20");
      todoList.addTask("Integrate with frontend", "Connect backend to UI", 4,
                       "2023-12-25");
      todoList.addTask("Urgent task", "This needs to be done right away", 5,
                       "2023-12-10");
      todoList.addTask("Low priority", "Can be done whenever", 1, "2024-01-15");
    }

    // Display existing tasks in creation order
    tasks = todoList.getTasks();
    std::cout << "\nTasks (in creation order):\n";
    for (const auto &task : tasks) {
      std::cout << "[" << task.id << "] " << task.header
                << " (Difficulty: " << task.difficulty
                << ", Due: " << task.dueDate << ")\n"
                << "    " << task.description << "\n";
    }

    // Display prioritized tasks
    std::cout << "\nTasks (prioritized by balanced algorithm):\n";
    auto prioritizedTasks = todoList.getPrioritizedTasks();
    for (const auto &task : prioritizedTasks) {
      std::cout << "[" << task.id << "] " << task.header
                << " (Difficulty: " << task.difficulty
                << ", Due: " << task.dueDate << ")\n"
                << "    " << task.description << "\n";
    }

    std::cout << "\nApplication ran successfully.\n";
    return 0;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
