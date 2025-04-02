#include "TaskPrioritizer.h"
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

TaskPrioritizer::TaskPrioritizer(Strategy strategy) : currentStrategy(strategy) {}

void TaskPrioritizer::setStrategy(Strategy strategy) {
    currentStrategy = strategy;
}

void TaskPrioritizer::prioritizeTasks(std::vector<Task>& tasks) {
    // Apply the chosen strategy
    switch (currentStrategy) {
        case Strategy::DUE_DATE_FIRST:
            sortByDueDate(tasks);
            break;
        case Strategy::DIFFICULTY_FIRST:
            sortByDifficulty(tasks);
            break;
        case Strategy::BALANCED:
        default:
            sortBalanced(tasks);
            break;
    }
}

// Helper function to convert date string to time_t for comparison
time_t stringToDate(const std::string& dateStr) {
    // If date is empty, return a very future date
    if (dateStr.empty()) {
        return std::numeric_limits<time_t>::max();
    }
    
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    
    // If date parsing failed, return a very future date
    if (ss.fail()) {
        return std::numeric_limits<time_t>::max();
    }
    
    return std::mktime(&tm);
}

void TaskPrioritizer::sortByDueDate(std::vector<Task>& tasks) const {
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        // First compare by due date
        time_t dateA = stringToDate(a.dueDate);
        time_t dateB = stringToDate(b.dueDate);
        
        if (dateA != dateB) {
            return dateA < dateB;  // Earlier date first
        }
        
        // If dates are the same, sort by difficulty (higher first)
        return a.difficulty > b.difficulty;
    });
}

void TaskPrioritizer::sortByDifficulty(std::vector<Task>& tasks) const {
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        // First compare by difficulty
        if (a.difficulty != b.difficulty) {
            return a.difficulty < b.difficulty;  // Easier tasks first
        }
        
        // If difficulties are equal, sort by due date
        time_t dateA = stringToDate(a.dueDate);
        time_t dateB = stringToDate(b.dueDate);
        return dateA < dateB;  // Earlier date first
    });
}

void TaskPrioritizer::sortBalanced(std::vector<Task>& tasks) const {
    // Get current time
    time_t now = std::time(nullptr);
    
    std::sort(tasks.begin(), tasks.end(), [now](const Task& a, const Task& b) {
        time_t dateA = stringToDate(a.dueDate);
        time_t dateB = stringToDate(b.dueDate);
        
        // Calculate days until due date
        double daysA = std::difftime(dateA, now) / (60 * 60 * 24);
        double daysB = std::difftime(dateB, now) / (60 * 60 * 24);
        
        // If a task is past due, give it highest priority
        if (daysA < 0) daysA = 0;
        if (daysB < 0) daysB = 0;
        
        // Calculate priority score (lower is higher priority)
        // Formula: days_until_due / difficulty
        // This means:
        // - Tasks with closer due dates get higher priority
        // - More difficult tasks get higher priority
        // - If a task has no due date, it gets lowest priority
        
        double scoreA = (dateA == std::numeric_limits<time_t>::max()) ? 
                        std::numeric_limits<double>::max() : 
                        daysA / a.difficulty;
                        
        double scoreB = (dateB == std::numeric_limits<time_t>::max()) ? 
                        std::numeric_limits<double>::max() : 
                        daysB / b.difficulty;
        
        return scoreA < scoreB;  // Lower score (higher priority) first
    });
}