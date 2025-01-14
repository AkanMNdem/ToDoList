#include "ToDoList.h"

ToDoList::ToDoList() : nextId(1) {}



void ToDoList::addTask(const std::string& description, int difficulty) {
    Task task = {nextId++, description, false, difficulty};
    tasks.push_back(task);
}

std::vector<Task> ToDoList::getTasks() const {
    return tasks;
}

bool ToDoList::markTaskAsCompleted(int id) {
    for (Task& task : tasks) {
        if (task.id == id) {
            task.completed = true;
            return true;
        }
    }
    return false;
}