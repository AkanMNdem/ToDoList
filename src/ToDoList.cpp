#include "ToDoList.h"
#include "TaskPrioritizer.h"
#include <stdexcept>
#include <iostream>

ToDoList::ToDoList() : db(nullptr, sqlite3_close) {}

void ToDoList::prepareStatements() {
    sqlite3_stmt* raw_stmt;
    
    // Add task
    if (sqlite3_prepare_v2(db.get(), 
        "INSERT INTO tasks (header, description, completed, difficulty, dueDate) VALUES (?, ?, ?, ?, ?)",
        -1, &raw_stmt, nullptr) == SQLITE_OK) {
        addTaskStmt.reset(raw_stmt);
    }
    
    // Prepare get tasks statement - now including dueDate
    if (sqlite3_prepare_v2(db.get(), 
        "SELECT id, header, description, completed, difficulty, dueDate FROM tasks WHERE completed = 0",
        -1, &raw_stmt, nullptr) == SQLITE_OK) {
        getTasksStmt.reset(raw_stmt);
    }
    
    // Delete task
    if (sqlite3_prepare_v2(db.get(), 
        "DELETE FROM tasks WHERE id = ?",
        -1, &raw_stmt, nullptr) == SQLITE_OK) {
        deleteTaskStmt.reset(raw_stmt);
    }
    
    // Edit task
    if (sqlite3_prepare_v2(db.get(), 
        "UPDATE tasks SET header = ?, description = ?, difficulty = ?, dueDate = ? WHERE id = ?",
        -1, &raw_stmt, nullptr) == SQLITE_OK) {
        editTaskStmt.reset(raw_stmt);
    }
    
    // Mark task as completed
    if (sqlite3_prepare_v2(db.get(), 
        "UPDATE tasks SET completed = 1 WHERE id = ?",
        -1, &raw_stmt, nullptr) == SQLITE_OK) {
        markCompletedStmt.reset(raw_stmt);
    }

    // Unmark task as completed
    if (sqlite3_prepare_v2(db.get(), 
        "UPDATE tasks SET completed = 0 WHERE id = ?",
        -1, &raw_stmt, nullptr) == SQLITE_OK) {
        unmarkCompletedStmt.reset(raw_stmt);
    }

    // Get completed tasks - now including dueDate
    if (sqlite3_prepare_v2(db.get(), 
        "SELECT id, header, description, completed, difficulty, dueDate FROM tasks WHERE completed = 1",
        -1, &raw_stmt, nullptr) == SQLITE_OK) {
        getCompletedTasksStmt.reset(raw_stmt);
    }
}

void ToDoList::connect(const std::string& dbPath) {
    sqlite3* raw_db;
    if (sqlite3_open(dbPath.c_str(), &raw_db) != SQLITE_OK) {
        throw std::runtime_error(std::string("Failed to open database: ") + sqlite3_errmsg(raw_db));
    }
    db.reset(raw_db); // smart pointer takes ownership of raw_db
    
    // Create tasks table if it doesn't exist
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "header TEXT NOT NULL,"
        "description TEXT,"
        "completed INTEGER DEFAULT 0,"
        "difficulty INTEGER CHECK(difficulty BETWEEN 1 AND 5),"
        "dueDate TEXT"
        ");";
        
    char* errorMsg = nullptr;
    if (sqlite3_exec(db.get(), createTableSQL, nullptr, nullptr, &errorMsg) != SQLITE_OK) {
        std::string error = "Failed to create table: ";
        error += errorMsg;
        sqlite3_free(errorMsg);
        throw std::runtime_error(error);
    }
    
    prepareStatements();
}

void ToDoList::addTask(const std::string& header, const std::string& description, int difficulty, const std::string& dueDate) {
    sqlite3_reset(addTaskStmt.get());
    sqlite3_bind_text(addTaskStmt.get(), 1, header.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(addTaskStmt.get(), 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(addTaskStmt.get(), 3, 0);  // not completed
    sqlite3_bind_int(addTaskStmt.get(), 4, difficulty);
    sqlite3_bind_text(addTaskStmt.get(), 5, dueDate.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(addTaskStmt.get()) != SQLITE_DONE) {
        throw std::runtime_error("Failed to insert task");
    }
}

void ToDoList::deleteTask(int id) {
    sqlite3_reset(deleteTaskStmt.get());
    sqlite3_bind_int(deleteTaskStmt.get(), 1, id);
    if (sqlite3_step(deleteTaskStmt.get()) != SQLITE_DONE) {
        throw std::runtime_error("Failed to delete task");
    }
}

void ToDoList::editTask(int id, const std::string& header, const std::string& description, int difficulty, const std::string& dueDate) {
    sqlite3_reset(editTaskStmt.get());
    sqlite3_bind_text(editTaskStmt.get(), 1, header.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(editTaskStmt.get(), 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(editTaskStmt.get(), 3, difficulty);
    sqlite3_bind_text(editTaskStmt.get(), 4, dueDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(editTaskStmt.get(), 5, id);

    if (sqlite3_step(editTaskStmt.get()) != SQLITE_DONE) {
        throw std::runtime_error("Failed to edit task");
    }
}

std::vector<Task> ToDoList::getTasks() const {
    sqlite3_reset(getTasksStmt.get());
    std::vector<Task> tasks;
    
    while (sqlite3_step(getTasksStmt.get()) == SQLITE_ROW) {
        Task task;
        task.id = sqlite3_column_int(getTasksStmt.get(), 0);
        
        // Handle text columns safely by checking for null
        const char* headerText = reinterpret_cast<const char*>(sqlite3_column_text(getTasksStmt.get(), 1));
        task.header = headerText ? std::string(headerText) : "";
        
        const char* descText = reinterpret_cast<const char*>(sqlite3_column_text(getTasksStmt.get(), 2));
        task.description = descText ? std::string(descText) : "";
        
        task.completed = sqlite3_column_int(getTasksStmt.get(), 3) != 0;
        task.difficulty = sqlite3_column_int(getTasksStmt.get(), 4);
        
        const char* dateText = reinterpret_cast<const char*>(sqlite3_column_text(getTasksStmt.get(), 5));
        task.dueDate = dateText ? std::string(dateText) : "";
        
        tasks.push_back(task);
    }
    
    return tasks;
}

bool ToDoList::markTaskAsCompleted(int id) {
    sqlite3_reset(markCompletedStmt.get());
    sqlite3_bind_int(markCompletedStmt.get(), 1, id);
    
    if (sqlite3_step(markCompletedStmt.get()) != SQLITE_DONE) {
        throw std::runtime_error("Failed to mark task as completed");
    }
    
    return sqlite3_changes(db.get()) > 0;
}

bool ToDoList::unmarkTaskAsCompleted(int id) {
    sqlite3_reset(unmarkCompletedStmt.get());
    sqlite3_bind_int(unmarkCompletedStmt.get(), 1, id);

    if (sqlite3_step(unmarkCompletedStmt.get()) != SQLITE_DONE) {
        throw std::runtime_error("Failed to unmark task as completed");
    }

    return sqlite3_changes(db.get()) > 0;
}

std::vector<Task> ToDoList::getCompletedTasks() const {
    sqlite3_reset(getCompletedTasksStmt.get());
    std::vector<Task> tasks;
    
    while (sqlite3_step(getCompletedTasksStmt.get()) == SQLITE_ROW) {
        Task task;
        task.id = sqlite3_column_int(getCompletedTasksStmt.get(), 0);
        
        const char* headerText = reinterpret_cast<const char*>(sqlite3_column_text(getCompletedTasksStmt.get(), 1));
        task.header = headerText ? std::string(headerText) : "";
        
        const char* descText = reinterpret_cast<const char*>(sqlite3_column_text(getCompletedTasksStmt.get(), 2));
        task.description = descText ? std::string(descText) : "";
        
        task.completed = sqlite3_column_int(getCompletedTasksStmt.get(), 3) != 0;
        task.difficulty = sqlite3_column_int(getCompletedTasksStmt.get(), 4);
        
        const char* dateText = reinterpret_cast<const char*>(sqlite3_column_text(getCompletedTasksStmt.get(), 5));
        task.dueDate = dateText ? std::string(dateText) : "";
        
        tasks.push_back(task);
    }
    
    return tasks;
}

std::vector<Task> ToDoList::getPrioritizedTasks() const {
    // Get all tasks
    auto tasks = getTasks();
    
    // Use TaskPrioritizer to sort them
    TaskPrioritizer prioritizer;
    prioritizer.prioritizeTasks(tasks);
    
    return tasks;
}