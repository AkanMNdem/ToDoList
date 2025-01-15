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
    ToDoList();
    ~ToDoList();
    void connect(const std::string& dbPath);
    void addTask(const std::string& description, int difficulty);
    std::vector<Task> getTasks() const;
    bool markTaskAsCompleted(int id);
    
private:
    sqlite3* db;
    std::vector<Task> tasks;
    int nextId;
};

#endif  