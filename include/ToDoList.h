#ifndef TODOLIST_H
#define TODOLIST_H

#include <string>
#include <sqlite3.h>

struct Task {
    int id;
    std::string description;
    bool completed;
    int difficulty;
};

class ToDoList { 
public:
    ToDoList(); // doesn't need to initialize db since we are using smart pointers
    //~ToDoList(); don't need destructor since we are using smart pointers
    void connect(const std::string& dbPath);
    void addTask(const std::string& description, int difficulty);
    std::vector<Task> getTasks() const;
    bool markTaskAsCompleted(int id);
    
private:
    // smart pointer for memory safety, prevents memory leaks before the destructor is called
    std::unique_ptr<sqlite3, decltype(&sqlite3_close)> db{nullptr, sqlite3_close};
    //sqlite3* db; raw pointer, not recommended for modern C++, can lead to memory leaks
    // std::vector<Task> tasks;
    // int nextId; Not needed since we are using the database, SQLite handles the IDs
};

#endif  