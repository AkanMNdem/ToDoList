#include <gtest/gtest.h>
#include "Task.h"

// Task class tests
TEST(TaskTest, DefaultConstructor) {
    Task task = {};  // Zero-initialize struct
    EXPECT_EQ(task.id, 0);
    EXPECT_TRUE(task.header.empty());
    EXPECT_TRUE(task.description.empty());
    EXPECT_FALSE(task.completed);
}

TEST(TaskTest, StructInitialization) {
    Task task = {
        1,                    // id
        "Test Task",          // header
        "This is a test task", // description
        false,                // completed
        2,                    // difficulty
        "2023-12-31"          // dueDate
    };
    
    EXPECT_EQ(task.id, 1);
    EXPECT_EQ(task.header, "Test Task");
    EXPECT_EQ(task.description, "This is a test task");
    EXPECT_FALSE(task.completed);
    EXPECT_EQ(task.difficulty, 2);
    EXPECT_EQ(task.dueDate, "2023-12-31");
}

TEST(TaskTest, FieldModification) {
    Task task = {};
    
    task.id = 5;
    EXPECT_EQ(task.id, 5);
    
    task.header = "Updated Title";
    EXPECT_EQ(task.header, "Updated Title");
    
    task.description = "Updated Description";
    EXPECT_EQ(task.description, "Updated Description");
    
    task.completed = true;
    EXPECT_TRUE(task.completed);
    
    task.difficulty = 3;
    EXPECT_EQ(task.difficulty, 3);
    
    task.dueDate = "2024-06-30";
    EXPECT_EQ(task.dueDate, "2024-06-30");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 