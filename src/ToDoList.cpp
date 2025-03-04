#include "ToDoList.h"
#include <stdexcept>

ToDoList::ToDoList() = default;

void ToDoList::prepareStatements() {
    sqlite3_stmt* raw_stmt;
    
    // Add task
    if (sqlite3_prepare_v2(db.get(), 
        "INSERT INTO tasks (header, description, completed, difficulty) VALUES (?, ?, ?, ?)",
        -1, &raw_stmt, nullptr) == SQLITE_OK) {
        addTaskStmt.reset(raw_stmt);
    }
    
    // Prepare get tasks statement
    if (sqlite3_prepare_v2(db.get(), "SELECT id, header,description, completed, difficulty FROM tasks",
                          -1, &raw_stmt, nullptr) == SQLITE_OK) {
        getTasksStmt.reset(raw_stmt);
    }
    
    // Delete task
    if (sqlite3_prepare_v2(db.get(), "DELETE FROM tasks WHERE id = ?",
                          -1, &raw_stmt, nullptr) == SQLITE_OK) {
        deleteTaskStmt.reset(raw_stmt);
    }
    
    // Edit task
    if (sqlite3_prepare_v2(db.get(), "UPDATE tasks SET header = ?, description = ?, difficulty = ? WHERE id = ?",
                          -1, &raw_stmt, nullptr) == SQLITE_OK) {
        editTaskStmt.reset(raw_stmt);
    }
    
    // Mark/unmark task as completed
    if (sqlite3_prepare_v2(db.get(), "UPDATE tasks SET completed = 1 WHERE id = ?",
                          -1, &raw_stmt, nullptr) == SQLITE_OK) {
        markCompletedStmt.reset(raw_stmt);
    }

    // Get completed tasks
    if (sqlite3_prepare_v2(db.get(), "SELECT id, header, description, completed, difficulty FROM tasks WHERE completed = 1",
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
    prepareStatements();
}

void ToDoList::addTask(const std::string& header, const std::string& description, int difficulty) {
    sqlite3_reset(addTaskStmt.get());
    sqlite3_bind_text(addTaskStmt.get(), 1, header.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(addTaskStmt.get(), 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(addTaskStmt.get(), 3, 0);
    sqlite3_bind_int(addTaskStmt.get(), 4, difficulty);
    
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
void ToDoList::editTask(int id, const std::string& header, const std::string& description, int difficulty) {
    sqlite3_reset(editTaskStmt.get());
    sqlite3_bind_text(editTaskStmt.get(), 1, header.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(editTaskStmt.get(), 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(editTaskStmt.get(), 3, difficulty);
    sqlite3_bind_int(editTaskStmt.get(), 4, id);

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
        task.header = std::string(reinterpret_cast<const char*>(sqlite3_column_text(getTasksStmt.get(), 1)));
        task.description = std::string(reinterpret_cast<const char*>(sqlite3_column_text(getTasksStmt.get(), 2)));
        task.completed = sqlite3_column_int(getTasksStmt.get(), 3) != 0;
        task.difficulty = sqlite3_column_int(getTasksStmt.get(), 4);
        tasks.push_back(task);
    }
    
    return tasks;
}

bool ToDoList::markTaskAsCompleted(int id) {
    sqlite3_reset(markCompletedStmt.get());
    sqlite3_bind_int(markCompletedStmt.get(), 1, 1);
    sqlite3_bind_int(markCompletedStmt.get(), 2, id);
    
    if (sqlite3_step(markCompletedStmt.get()) != SQLITE_DONE) {
        throw std::runtime_error("Failed to mark task as completed");
    }
    
    return sqlite3_changes(db.get()) > 0;
}

bool ToDoList::unmarkTaskAsCompleted(int id) {
    sqlite3_reset(unmarkCompletedStmt.get());
    sqlite3_bind_int(unmarkCompletedStmt.get(), 1, 0);
    sqlite3_bind_int(unmarkCompletedStmt.get(), 2, id);

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
        task.header = std::string(reinterpret_cast<const char*>(sqlite3_column_text(getCompletedTasksStmt.get(), 1)));
        task.description = std::string(reinterpret_cast<const char*>(sqlite3_column_text(getCompletedTasksStmt.get(), 2)));
        task.completed = sqlite3_column_int(getCompletedTasksStmt.get(), 3) != 0;
        task.difficulty = sqlite3_column_int(getCompletedTasksStmt.get(), 4);
        tasks.push_back(task);
    }
    
    return tasks;
}