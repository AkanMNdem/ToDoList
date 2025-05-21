#include "Task.h"
#include "TaskPrioritizer.h"
#include "ToDoList.h"
#include <drogon/drogon.h>
#include <filesystem>
#include <string>
#include <vector>
#include <ctime>  // For std::time

// Using directives for Drogon types
using drogon::app;
using drogon::Get;
using drogon::Post;
using drogon::Delete;
using drogon::Put;
using drogon::Options;  // Add Options method
using drogon::HttpRequestPtr;
using drogon::HttpResponsePtr;
using drogon::HttpResponse;
using drogon::k200OK;
using drogon::k201Created;
using drogon::k204NoContent;
using drogon::k400BadRequest;
using drogon::k404NotFound;
using drogon::k500InternalServerError;

// Helper function to convert Task to JSON
Json::Value taskToJson(const Task &task) {
    Json::Value json;
    json["id"] = task.id;
    json["header"] = task.header;
    json["description"] = task.description;
    json["completed"] = task.completed;
    json["difficulty"] = task.difficulty;
    json["dueDate"] = task.dueDate;
    return json;
}

int main() {
    // Ensure data directory exists
    std::filesystem::create_directories("data");

    // Initialize database connection
    ToDoList todoList;
    try {
        todoList.connect("data/tasks.db");
        std::cout << "Connected to database successfully.\n";
    } catch (const std::exception &e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        return 1;
    }

    // Health check endpoint for monitoring
    app().registerHandler("/health", 
        [](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
            Json::Value result;
            result["status"] = "ok";
            result["timestamp"] = static_cast<Json::Int64>(std::time(nullptr));
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        },
        {Get});

    // GET all tasks
    app().registerHandler("/api/tasks", 
        [&todoList](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
            try {
                auto tasks = todoList.getTasks();
                Json::Value result;
                Json::Value taskList(Json::arrayValue);

                for (const auto &task : tasks) {
                    taskList.append(taskToJson(task));
                }
                result["tasks"] = taskList;

                auto resp = HttpResponse::newHttpJsonResponse(result);
                callback(resp);
            } catch (const std::exception &e) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody(std::string("Error: ") + e.what());
                callback(resp);
            }
        },
        {Get});

    // GET a single task by ID
    app().registerHandler("/api/tasks/{id}", 
        [&todoList](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &id) {
            try {
                std::cout << "Received request for task ID: " << id << std::endl;
                
                int taskId = std::stoi(id);
                std::cout << "Converted to int: " << taskId << std::endl;
                
                auto tasks = todoList.getTasks();
                std::cout << "Found " << tasks.size() << " total tasks" << std::endl;
                
                for (const auto &task : tasks) {
                    if (task.id == taskId) {
                        std::cout << "Found matching task with ID " << taskId << std::endl;
                        auto resp = HttpResponse::newHttpJsonResponse(taskToJson(task));
                        callback(resp);
                        return;
                    }
                }
                
                std::cout << "No task found with ID " << taskId << std::endl;
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k404NotFound);
                resp->setBody("Task not found");
                callback(resp);
            } catch (const std::exception &e) {
                std::cerr << "Error processing request: " << e.what() << std::endl;
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody(std::string("Error: ") + e.what());
                callback(resp);
            }
        },
        {Get});

    // GET all completed tasks
    app().registerHandler("/api/tasks/completed", 
        [&todoList](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
            try {
                auto tasks = todoList.getCompletedTasks();
                Json::Value result;
                Json::Value taskList(Json::arrayValue);

                for (const auto &task : tasks) {
                    taskList.append(taskToJson(task));
                }
                result["tasks"] = taskList;

                auto resp = HttpResponse::newHttpJsonResponse(result);
                callback(resp);
            } catch (const std::exception &e) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody(std::string("Error: ") + e.what());
                callback(resp);
            }
        },
        {Get});

    // GET prioritized tasks
    app().registerHandler("/api/tasks/prioritized", 
        [&todoList](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
            try {
                auto tasks = todoList.getPrioritizedTasks();
                Json::Value result;
                Json::Value taskList(Json::arrayValue);

                for (const auto &task : tasks) {
                    taskList.append(taskToJson(task));
                }
                result["tasks"] = taskList;

                auto resp = HttpResponse::newHttpJsonResponse(result);
                callback(resp);
            } catch (const std::exception &e) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody(std::string("Error: ") + e.what());
                callback(resp);
            }
        },
        {Get});

    // POST new task
    app().registerHandler("/api/tasks", 
        [&todoList](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
            try {
                auto json = req->getJsonObject();
                if (!json || !json->isMember("header") || !json->isMember("difficulty")) {
                    auto resp = HttpResponse::newHttpResponse();
                    resp->setStatusCode(k400BadRequest);
                    resp->setBody("Missing required fields");
                    callback(resp);
                    return;
                }

                int difficulty = (*json)["difficulty"].asInt();
                if (difficulty < 1 || difficulty > 5) {
                    auto resp = HttpResponse::newHttpResponse();
                    resp->setStatusCode(k400BadRequest);
                    resp->setBody("Difficulty must be between 1 and 5");
                    callback(resp);
                    return;
                }

                std::string header = (*json)["header"].asString();
                std::string description = json->isMember("description") ? (*json)["description"].asString() : "";
                std::string dueDate = json->isMember("dueDate") ? (*json)["dueDate"].asString() : "";

                if (!dueDate.empty()) {
                    if (dueDate.length() != 10 || dueDate[4] != '-' || dueDate[7] != '-') {
                        auto resp = HttpResponse::newHttpResponse();
                        resp->setStatusCode(k400BadRequest);
                        resp->setBody("Due date must be in YYYY-MM-DD format");
                        callback(resp);
                        return;
                    }
                }

                todoList.addTask(header, description, difficulty, dueDate);
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k201Created);
                callback(resp);
            } catch (const std::exception &e) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody(std::string("Error: ") + e.what());
                callback(resp);
            }
        },
        {Post});

    // DELETE task
    app().registerHandler("/api/tasks/{id}", 
        [&todoList](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &id) {
            try {
                int taskId = std::stoi(id);
                todoList.deleteTask(taskId);
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k204NoContent);
                callback(resp);
            } catch (const std::exception &e) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody(std::string("Error: ") + e.what());
                callback(resp);
            }
        },
        {Delete});

    // PUT update task
    app().registerHandler("/api/tasks/{id}", 
        [&todoList](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &id) {
            try {
                int taskId = std::stoi(id);
                auto json = req->getJsonObject();
                if (!json || !json->isMember("header") || !json->isMember("difficulty")) {
                    auto resp = HttpResponse::newHttpResponse();
                    resp->setStatusCode(k400BadRequest);
                    resp->setBody("Missing required fields");
                    callback(resp);
                    return;
                }

                int difficulty = (*json)["difficulty"].asInt();
                if (difficulty < 1 || difficulty > 5) {
                    auto resp = HttpResponse::newHttpResponse();
                    resp->setStatusCode(k400BadRequest);
                    resp->setBody("Difficulty must be between 1 and 5");
                    callback(resp);
                    return;
                }

                std::string header = (*json)["header"].asString();
                std::string description = json->isMember("description") ? (*json)["description"].asString() : "";
                std::string dueDate = json->isMember("dueDate") ? (*json)["dueDate"].asString() : "";

                if (!dueDate.empty()) {
                    if (dueDate.length() != 10 || dueDate[4] != '-' || dueDate[7] != '-') {
                        auto resp = HttpResponse::newHttpResponse();
                        resp->setStatusCode(k400BadRequest);
                        resp->setBody("Due date must be in YYYY-MM-DD format");
                        callback(resp);
                        return;
                    }
                }

                todoList.editTask(taskId, header, description, difficulty, dueDate);
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k200OK);
                callback(resp);
            } catch (const std::exception &e) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody(std::string("Error: ") + e.what());
                callback(resp);
            }
        },
        {Put});

    // POST mark task as completed
    app().registerHandler("/api/tasks/{id}/complete", 
        [&todoList](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &id) {
            try {
                int taskId = std::stoi(id);
                bool marked = todoList.markTaskAsCompleted(taskId);
                auto resp = HttpResponse::newHttpResponse();
                if (marked) {
                    resp->setStatusCode(k200OK);
                } else {
                    resp->setStatusCode(k404NotFound);
                    resp->setBody("Task not found");
                }
                callback(resp);
            } catch (const std::exception &e) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody(std::string("Error: ") + e.what());
                callback(resp);
            }
        },
        {Post});

    // POST unmark task as completed
    app().registerHandler("/api/tasks/{id}/uncomplete", 
        [&todoList](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &id) {
            try {
                int taskId = std::stoi(id);
                bool unmarked = todoList.unmarkTaskAsCompleted(taskId);
                auto resp = HttpResponse::newHttpResponse();
                if (unmarked) {
                    resp->setStatusCode(k200OK);
                } else {
                    resp->setStatusCode(k404NotFound);
                    resp->setBody("Task not found");
                }
                callback(resp);
            } catch (const std::exception &e) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody(std::string("Error: ") + e.what());
                callback(resp);
            }
        },
        {Post});

    // GET prioritization strategies
    app().registerHandler("/api/prioritization/strategies", 
        [](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
            Json::Value result;
            Json::Value strategies(Json::arrayValue);

            Json::Value dueDateFirst;
            dueDateFirst["id"] = static_cast<int>(TaskPrioritizer::Strategy::DUE_DATE_FIRST);
            dueDateFirst["name"] = "Due Date First";
            dueDateFirst["description"] = "Prioritize tasks by due date, then by difficulty";

            Json::Value difficultyFirst;
            difficultyFirst["id"] = static_cast<int>(TaskPrioritizer::Strategy::DIFFICULTY_FIRST);
            difficultyFirst["name"] = "Difficulty First";
            difficultyFirst["description"] = "Prioritize easier tasks first, then by due date";

            Json::Value balanced;
            balanced["id"] = static_cast<int>(TaskPrioritizer::Strategy::BALANCED);
            balanced["name"] = "Balanced";
            balanced["description"] = "Use a weighted algorithm considering both due date and difficulty";

            strategies.append(dueDateFirst);
            strategies.append(difficultyFirst);
            strategies.append(balanced);

            result["strategies"] = strategies;
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        },
        {Get});

    // Start the server
    std::cout << "Starting API server on http://localhost:8080\n";
    app().setLogLevel(trantor::Logger::kWarn);
    app().addListener("0.0.0.0", 8080);
    // Add CORS headers
    app().registerHandler("/.*", [](const HttpRequestPtr& req, 
                                std::function<void(const HttpResponsePtr&)>&& callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "OPTIONS, GET, POST, PUT, DELETE");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
        callback(resp);
    }, {Options});
    app().run();

    return 0;
}
