#ifndef TASK_PRIORITIZER_H
#define TASK_PRIORITIZER_H

#include <vector>
#include "Task.h"

class TaskPrioritizer {
public:
    // Different prioritization strategies
    enum class Strategy {
        DUE_DATE_FIRST,    // Prioritize by due date, then difficulty
        DIFFICULTY_FIRST,  // Prioritize by difficulty, then due date
        BALANCED           // Use a weighted algorithm
    };
    
    TaskPrioritizer(Strategy strategy = Strategy::BALANCED);
    void setStrategy(Strategy strategy);
    
    // Sorts tasks in place by priority
    void prioritizeTasks(std::vector<Task>& tasks);
    
private:
    Strategy currentStrategy;
    // Helper methods for different strategies
    void sortByDueDate(std::vector<Task>& tasks) const;
    void sortByDifficulty(std::vector<Task>& tasks) const;
    void sortBalanced(std::vector<Task>& tasks) const;
};

#endif