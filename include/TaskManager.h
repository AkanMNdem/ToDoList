#ifndef TASKAMANGER_H
#define TASKAMANGER_H

#include <string>

struct Task {
    int id;
    std::string description;
    bool completed;
    int difficulty;
};

class TaskManager {
public:
    TaskManager();
    void addTask(const std::string& description, int difficulty);
    std::vector<Task> getTasks();
private:
    std::vector<Task> tasks;
    int nextId;
};

#endif  