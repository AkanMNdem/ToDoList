#include "ToDoList.h"
#include "TaskPrioritizer.h"
#include <gtest/gtest.h>
#include <filesystem>

class ToDoListTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Use temporary database for testing
        dbPath = "test_tasks.db";
        todoList.connect(dbPath);
    }
    
    void TearDown() override {
        // Remove test database
        std::filesystem::remove(dbPath);
    }
    
    ToDoList todoList;
    std::string dbPath;
};

TEST_F(ToDoListTest, AddAndRetrieveTask) {
    todoList.addTask("Test Task", "Description", 3, "2023-12-31");
    auto tasks = todoList.getTasks();
    
    ASSERT_EQ(1, tasks.size());
    EXPECT_EQ("Test Task", tasks[0].header);
    EXPECT_EQ("Description", tasks[0].description);
    EXPECT_EQ(3, tasks[0].difficulty);
    EXPECT_EQ("2023-12-31", tasks[0].dueDate);
    EXPECT_FALSE(tasks[0].completed);
}