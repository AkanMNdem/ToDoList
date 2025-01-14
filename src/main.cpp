#include "ToDoList.h"
#include <iostream>
#include <string>

void displayMenu() {
    std::cout << "\nTo-Do List Application\n" << std::endl;
    std::cout << "1. Add Task" << std::endl;
    std::cout << "2. View Tasks" << std::endl;
    std::cout << "3. Mark Task as Completed" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void printTasks(const std::vector<Task>& tasks) {
    for (const auto& task : tasks) {
        std::cout << "ID: " << task.id 
        << ", Description: " << task.description
        << ", Difficulty: " << task.difficulty << std::endl
        << ", Completed: " << (task.completed ? "Yes" : "No") << std::endl;
    }
}

int main() {
    ToDoList todoList;
    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::cin.ignore();
                std::string description;
                int difficulty;

                std::cout << "Enter task description: ";
                std::getline(std::cin, description);
                std::cout << "Enter task difficulty (1-5): ";
                std::cin >> difficulty;

                todoList.addTask(description, difficulty);
                std::cout << "Task added successfully!" << std::endl;
                break;
            }
            case 2: {
                std::vector<Task> tasks = todoList.getTasks();
                if (tasks.empty()) {
                    std::cout << "No Tasks available\n";
                } else {
                    printTasks(tasks);
                }
                break;
            }
            case 3: {
                int id;
                std::cout << "Enter task ID to mark as completed: ";
                std::cin >> id;
                if (todoList.markTaskAsCompleted(id)) {
                    std::cout << "Task marked as completed.\n";
                } else {
                    std::cout << "Task not found.\n";
                }
                break;
            }
            case 4:
                std::cout << "Exiting program.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);
    
    return 0;
}
