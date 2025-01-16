CREATE TABLE tasks (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    header TEXT NOT NULL,
    description TEXT NOT NULL,
    completed BOOLEAN NOT NULL DEFAULT 0,
    difficulty INTEGER NOT NULL CHECK (difficulty BETWEEN 1 AND 5),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    -- due_date TEXT,  // We can add this later
    -- category TEXT,  // We can add this later
);