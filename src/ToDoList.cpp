#include "ToDoList.h"
#include <stdexcept>

ToDoList::ToDoList() : nextId(1) {}
ToDoList::ToDoList() : db(nullptr) {}

ToDoList::~ToDoList() {
    if (db) {
        sqlite3_close(db);
    }
}

void ToDoList::connect(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open database");
    }
}

void ToDoList::addTask(const std::string& description, int difficulty) {
    const std::string sql = "INSERT INTO tasks (description, completed, difficulty) VALUES (?, ?,?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }
    sqlite3_bind_text(stmt, 1, description.c_str(), description.size(), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, 0);
    sqlite3_bind_int(stmt, 3, difficulty);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw std::runtime_error("Failed to insert task");
    }
    sqlite3_finalize(stmt);
}

std::vector<Task> ToDoList::getTasks() const {
    const char* sql = "SELECT id, description, completed, difficulty FROM tasks";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }
    std::vector<Task> tasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Task task;
        task.id = sqlite3_column_int(stmt, 0);
        task.description = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        task.completed = sqlite3_column_int(stmt, 2) != 0;
        task.difficulty = sqlite3_column_int(stmt, 3);
        tasks.push_back(task);
    }
    sqlite3_finalize(stmt);
    return tasks;
}

bool ToDoList::markTaskAsCompleted(int id) {
    const std::string sql = "UPDATE tasks SET completed = 1 WHERE id = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw std::runtime_error("Failed to mark task as completed");
    }
    sqlite3_finalize(stmt);
            return true;
        }
    }
    return false;
}