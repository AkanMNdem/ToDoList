#include "Task.h"
#include "ToDoList.h"
#include <crow.h>
#include <string>
#include <vector>

// Helper function to convert Task to JSON
crow::json::wvalue taskToJson(const Task &task) {
  crow::json::wvalue json;
  json["id"] = task.id;
  json["header"] = task.header;
  json["description"] = task.description;
  json["completed"] = task.completed;
  json["difficulty"] = task.difficulty;
  json["dueDate"] = task.dueDate;
  return json;
}

int main() {
  // Initialize database connection
  ToDoList todoList;
  try {
    todoList.connect("data/tasks.db");
    std::cout << "Connected to database successfully.\n";
  } catch (const std::exception &e) {
    std::cerr << "Database error: " << e.what() << std::endl;
    return 1;
  }

  crow::SimpleApp app;

  // Enable CORS for development
  auto &cors = app.get_middleware<crow::CORSHandler>();
  cors.global()
      .headers("Content-Type")
      .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method);

  // GET all tasks
  CROW_ROUTE(app, "/api/tasks").methods("GET"_method)([&todoList]() {
    try {
      auto tasks = todoList.getTasks();
      crow::json::wvalue result;
      crow::json::wvalue::list taskList;

      for (const auto &task : tasks) {
        taskList.push_back(taskToJson(task));
      }
      result["tasks"] = std::move(taskList);
      return crow::response(result);
    } catch (const std::exception &e) {
      return crow::response(500, std::string("Error: ") + e.what());
    }
  });

  // GET all completed tasks
  CROW_ROUTE(app, "/api/tasks/completed").methods("GET"_method)([&todoList]() {
    try {
      auto tasks = todoList.getCompletedTasks();
      crow::json::wvalue result;
      crow::json::wvalue::list taskList;

      for (const auto &task : tasks) {
        taskList.push_back(taskToJson(task));
      }
      result["tasks"] = std::move(taskList);
      return crow::response(result);
    } catch (const std::exception &e) {
      return crow::response(500, std::string("Error: ") + e.what());
    }
  });

  // GET prioritized tasks
  CROW_ROUTE(app, "/api/tasks/prioritized")
      .methods("GET"_method)([&todoList]() {
        try {
          auto tasks = todoList.getPrioritizedTasks();
          crow::json::wvalue result;
          crow::json::wvalue::list taskList;

          for (const auto &task : tasks) {
            taskList.push_back(taskToJson(task));
          }
          result["tasks"] = std::move(taskList);
          return crow::response(result);
        } catch (const std::exception &e) {
          return crow::response(500, std::string("Error: ") + e.what());
        }
      });

  // POST new task
  CROW_ROUTE(app, "/api/tasks")
      .methods("POST"_method)([&todoList](const crow::request &req) {
        try {
          auto json = crow::json::load(req.body);
          if (!json) {
            return crow::response(400, "Invalid JSON");
          }

          if (!json.has("header") || !json.has("difficulty")) {
            return crow::response(400, "Missing required fields");
          }

          std::string header = json["header"].s();
          std::string description =
              json.has("description") ? json["description"].s() : "";
          int difficulty = json["difficulty"].i();
          std::string dueDate = json.has("dueDate") ? json["dueDate"].s() : "";

          todoList.addTask(header, description, difficulty, dueDate);
          return crow::response(201);
        } catch (const std::exception &e) {
          return crow::response(500, std::string("Error: ") + e.what());
        }
      });

  // DELETE task
  CROW_ROUTE(app, "/api/tasks/<int>")
      .methods("DELETE"_method)([&todoList](int id) {
        try {
          todoList.deleteTask(id);
          return crow::response(204);
        } catch (const std::exception &e) {
          return crow::response(500, std::string("Error: ") + e.what());
        }
      });

  // PUT update task
  CROW_ROUTE(app, "/api/tasks/<int>")
      .methods("PUT"_method)([&todoList](const crow::request &req, int id) {
        try {
          auto json = crow::json::load(req.body);
          if (!json) {
            return crow::response(400, "Invalid JSON");
          }

          if (!json.has("header") || !json.has("difficulty")) {
            return crow::response(400, "Missing required fields");
          }

          std::string header = json["header"].s();
          std::string description =
              json.has("description") ? json["description"].s() : "";
          int difficulty = json["difficulty"].i();
          std::string dueDate = json.has("dueDate") ? json["dueDate"].s() : "";

          todoList.editTask(id, header, description, difficulty, dueDate);
          return crow::response(200);
        } catch (const std::exception &e) {
          return crow::response(500, std::string("Error: ") + e.what());
        }
      });

  // POST mark task as completed
  CROW_ROUTE(app, "/api/tasks/<int>/complete")
      .methods("POST"_method)([&todoList](int id) {
        try {
          bool marked = todoList.markTaskAsCompleted(id);
          if (marked) {
            return crow::response(200);
          } else {
            return crow::response(404, "Task not found");
          }
        } catch (const std::exception &e) {
          return crow::response(500, std::string("Error: ") + e.what());
        }
      });

  // POST unmark task as completed
  CROW_ROUTE(app, "/api/tasks/<int>/uncomplete")
      .methods("POST"_method)([&todoList](int id) {
        try {
          bool unmarked = todoList.unmarkTaskAsCompleted(id);
          if (unmarked) {
            return crow::response(200);
          } else {
            return crow::response(404, "Task not found");
          }
        } catch (const std::exception &e) {
          return crow::response(500, std::string("Error: ") + e.what());
        }
      });

  // Start the server
  std::cout << "Starting API server on http://localhost:8080\n";
  app.port(8080).run();

  return 0;
}
