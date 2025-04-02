#ifndef TODOLIST_H
#define TODOLIST_H

#include <string>
#include <vector>
#include <memory>
#include <sqlite3.h>
#include "Task.h"

class ToDoList { 
public:
    ToDoList();
    void connect(const std::string& dbPath);
    
    // Basic CRUD operations
    void addTask(const std::string& header, const std::string& description, int difficulty, const std::string& dueDate);
    void deleteTask(int id);
    void editTask(int id, const std::string& header, const std::string& description, int difficulty, const std::string& dueDate);
    std::vector<Task> getTasks() const;  // By creation order
    
    // Task status
    bool markTaskAsCompleted(int id);
    bool unmarkTaskAsCompleted(int id);  // New "undo complete" feature
    std::vector<Task> getCompletedTasks() const;  // View completed tasks
    
    // Task prioritization
    std::vector<Task> getPrioritizedTasks() const;
    
private:
    // Smart pointer for memory safety, prevents memory leaks before the destructor is called
    std::unique_ptr<sqlite3, decltype(&sqlite3_close)> db{nullptr, sqlite3_close};

    // Cached statements
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> addTaskStmt{nullptr, sqlite3_finalize};
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> getTasksStmt{nullptr, sqlite3_finalize};
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> markCompletedStmt{nullptr, sqlite3_finalize};
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> deleteTaskStmt{nullptr, sqlite3_finalize};
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> editTaskStmt{nullptr, sqlite3_finalize};
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> unmarkCompletedStmt{nullptr, sqlite3_finalize};
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> getCompletedTasksStmt{nullptr, sqlite3_finalize};
    
    void prepareStatements(); // Initialize prepared statements
};

#endif