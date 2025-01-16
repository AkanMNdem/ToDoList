#ifndef STATEMENT_H
#define STATEMENT_H

#include <sqlite3.h>
#include <string>
#include <memory>

// A wrapper class to manage sqlite3_stmt* safely
class Statement {
private:
    // Smart pointer that automatically calls sqlite3_finalize when destroyed
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt{nullptr, sqlite3_finalize};
    
public:
    Statement(sqlite3* db, const std::string& sql) {
        sqlite3_stmt* raw_stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &raw_stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare statement");
        }
        stmt.reset(raw_stmt);
    }

    void bindInt(int pos, int value) {
        if (sqlite3_bind_int(stmt.get(), pos, value) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind int");
        }
    }

    void bindText(int pos, const std::string& value) {
        if (sqlite3_bind_text(stmt.get(), pos, value.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind text");
        }
    }

    bool step() {
        int result = sqlite3_step(stmt.get());
        if (result == SQLITE_DONE) return false;
        if (result == SQLITE_ROW) return true;
        throw std::runtime_error("Error executing statement");
    }
    
    // Getters for reading results
    int getInt(int col) { return sqlite3_column_int(stmt.get(), col); }
    std::string getText(int col) {
        auto text = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), col));
        return text ? text : "";
    }

    // Reset statement for reuse
    void reset() {
        if (sqlite3_reset(stmt.get()) != SQLITE_OK) {
            throw std::runtime_error("Failed to reset statement");
        }
    }
};

#endif