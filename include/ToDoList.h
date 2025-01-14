#ifndef TODOLIST_H
#define TODOLIST_H

#include <string>

struct Task {
    int id;
    std::string description;
    bool completed;
    int difficulty;
};

class ToDoList { 
public:
    ToDoList();
    void addTask(const std::string& description, int difficulty);
    std::vector<Task> getTasks() const;
    bool markTaskAsCompleted(int id);
private:
    std::vector<Task> tasks;
    int nextId;
};

#endif  