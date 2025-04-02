// Task.h
#ifndef TASK_H
#define TASK_H

#include <string>

struct Task {
    int id;
    std::string header;
    std::string description;
    bool completed;
    int difficulty;
    std::string dueDate;
};

#endif