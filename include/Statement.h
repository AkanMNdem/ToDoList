#ifndef STATEMENT_H
#define STATEMENT_H

#include <sqlite3.h>
#include <string>
#include <memory>

// A wrapper class to manage sqlite3_stmt* safely
class Statement {
private:
    // Smart pointer that automatically calls sqlite3_finalize when destroyed
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt;
    
public:
    // Constructor takes raw sqlite3_stmt* and wraps it
    explicit Statement(sqlite3_stmt* stmt) 
        : stmt(stmt, sqlite3_finalize) {}  // Second argument is the deleter function
    
    // Get underlying pointer when we need to use it with SQLite functions
    sqlite3_stmt* get() { return stmt.get(); }
    
    // No copying allowed (smart pointers can't be copied)
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;
};

#endif