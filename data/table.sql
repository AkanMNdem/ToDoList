CREATE TABLE tasks (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    description TEXT NOT NULL,
    complete BOOLEAN NOT NULL DEFAULT 0,
    difficulty INTEGER NOT NULL,
    due_date TEXT
);