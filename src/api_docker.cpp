#include "Task.h"
#include "TaskPrioritizer.h"
#include "ToDoList.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <ctime>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

// Simple JSON helper functions
std::string escapeJsonString(const std::string& s) {
    std::ostringstream o;
    for (auto c = s.cbegin(); c != s.cend(); c++) {
        if (*c == '"' || *c == '\\' || ('\x00' <= *c && *c <= '\x1f')) {
            o << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(*c);
        } else {
            o << *c;
        }
    }
    return o.str();
}

std::string taskToJson(const Task& task) {
    std::ostringstream ss;
    ss << "{";
    ss << "\"id\":" << task.id << ",";
    ss << "\"header\":\"" << escapeJsonString(task.header) << "\",";
    ss << "\"description\":\"" << escapeJsonString(task.description) << "\",";
    ss << "\"completed\":" << (task.completed ? "true" : "false") << ",";
    ss << "\"difficulty\":" << task.difficulty << ",";
    ss << "\"dueDate\":\"" << escapeJsonString(task.dueDate) << "\"";
    ss << "}";
    return ss.str();
}

std::string tasksToJson(const std::vector<Task>& tasks) {
    std::ostringstream ss;
    ss << "{\"tasks\":[";
    
    for (size_t i = 0; i < tasks.size(); ++i) {
        if (i > 0) {
            ss << ",";
        }
        ss << taskToJson(tasks[i]);
    }
    
    ss << "]}";
    return ss.str();
}

// HTTP Response helpers
std::string makeHttpResponse(int statusCode, const std::string& statusText, const std::string& contentType, const std::string& body) {
    std::ostringstream response;
    response << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "Access-Control-Allow-Origin: *\r\n";
    response << "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
    response << "Access-Control-Allow-Headers: Content-Type\r\n";
    response << "\r\n";
    response << body;
    return response.str();
}

std::string okJson(const std::string& json) {
    return makeHttpResponse(200, "OK", "application/json", json);
}

std::string created() {
    return makeHttpResponse(201, "Created", "text/plain", "");
}

std::string noContent() {
    return makeHttpResponse(204, "No Content", "text/plain", "");
}

std::string badRequest(const std::string& message) {
    return makeHttpResponse(400, "Bad Request", "text/plain", message);
}

std::string notFound(const std::string& message = "Not Found") {
    return makeHttpResponse(404, "Not Found", "text/plain", message);
}

std::string options() {
    return makeHttpResponse(200, "OK", "text/plain", "");
}

// Signal handler for graceful shutdown
bool running = true;
void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    running = false;
}

// Parse HTTP request
struct HttpRequest {
    std::string method;
    std::string path;
    std::string body;
};

HttpRequest parseRequest(const std::string& requestStr) {
    HttpRequest request;
    
    // Extract method and path
    size_t methodEnd = requestStr.find(' ');
    if (methodEnd != std::string::npos) {
        request.method = requestStr.substr(0, methodEnd);
        size_t pathEnd = requestStr.find(' ', methodEnd + 1);
        if (pathEnd != std::string::npos) {
            request.path = requestStr.substr(methodEnd + 1, pathEnd - methodEnd - 1);
        }
    }
    
    // Extract body
    size_t bodyStart = requestStr.find("\r\n\r\n");
    if (bodyStart != std::string::npos) {
        request.body = requestStr.substr(bodyStart + 4);
    }
    
    return request;
}

// Parse path parameters
std::string getPathParam(const std::string& path, const std::string& prefix) {
    if (path.rfind(prefix, 0) == 0) {
        std::string rest = path.substr(prefix.length());
        // Remove trailing slash if present
        if (!rest.empty() && rest.back() == '/') {
            rest.pop_back();
        }
        return rest;
    }
    return "";
}

int extractTaskId(const std::string& pathParam) {
    try {
        return std::stoi(pathParam);
    } catch (const std::exception&) {
        return -1;
    }
}

// Main application
int main() {
    // Register signal handler
    signal(SIGINT, signalHandler);
    
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
    
    // Create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
    
    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Error setting socket options" << std::endl;
        return 1;
    }
    
    // Setup address struct
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    
    // Bind socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }
    
    // Listen for connections
    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Error listening" << std::endl;
        return 1;
    }
    
    std::cout << "Starting API server on http://localhost:8080\n";
    
    // Main loop
    while (running) {
        // Accept connection
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        
        if (clientSocket < 0) {
            if (running) {
                std::cerr << "Error accepting connection" << std::endl;
            }
            continue;
        }
        
        // Read request
        char buffer[4096] = {0};
        ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
        
        if (bytesRead <= 0) {
            close(clientSocket);
            continue;
        }
        
        // Parse request
        std::string requestStr(buffer);
        HttpRequest request = parseRequest(requestStr);
        
        // Process request
        std::string response;
        
        // Health endpoint
        if (request.path == "/health" && request.method == "GET") {
            std::ostringstream health;
            health << "{\"status\":\"ok\",\"timestamp\":" << std::time(nullptr) << "}";
            response = okJson(health.str());
        }
        // Handle OPTIONS requests
        else if (request.method == "OPTIONS") {
            response = options();
        }
        // Get all tasks
        else if (request.path == "/api/tasks" && request.method == "GET") {
            try {
                auto tasks = todoList.getTasks();
                response = okJson(tasksToJson(tasks));
            } catch (const std::exception& e) {
                response = badRequest(e.what());
            }
        }
        // Get a single task
        else if (request.path.rfind("/api/tasks/", 0) == 0 && request.method == "GET") {
            // Extract task ID
            std::string pathParam = getPathParam(request.path, "/api/tasks/");
            
            // Check for special paths
            if (pathParam == "completed") {
                try {
                    auto tasks = todoList.getCompletedTasks();
                    response = okJson(tasksToJson(tasks));
                } catch (const std::exception& e) {
                    response = badRequest(e.what());
                }
            } else if (pathParam == "prioritized") {
                try {
                    auto tasks = todoList.getPrioritizedTasks();
                    response = okJson(tasksToJson(tasks));
                } catch (const std::exception& e) {
                    response = badRequest(e.what());
                }
            } else {
                int taskId = extractTaskId(pathParam);
                if (taskId < 0) {
                    response = badRequest("Invalid task ID");
                } else {
                    auto tasks = todoList.getTasks();
                    bool found = false;
                    
                    for (const auto& task : tasks) {
                        if (task.id == taskId) {
                            response = okJson(taskToJson(task));
                            found = true;
                            break;
                        }
                    }
                    
                    if (!found) {
                        response = notFound("Task not found");
                    }
                }
            }
        }
        // Add a new task
        else if (request.path == "/api/tasks" && request.method == "POST") {
            // Parse JSON body
            try {
                // Very basic JSON parsing - in a real app, use a proper JSON library
                size_t headerPos = request.body.find("\"header\":");
                size_t difficultyPos = request.body.find("\"difficulty\":");
                
                if (headerPos == std::string::npos || difficultyPos == std::string::npos) {
                    response = badRequest("Missing required fields");
                } else {
                    // Extract header
                    size_t headerStart = request.body.find("\"", headerPos + 9) + 1;
                    size_t headerEnd = request.body.find("\"", headerStart);
                    std::string header = request.body.substr(headerStart, headerEnd - headerStart);
                    
                    // Extract difficulty
                    size_t difficultyStart = difficultyPos + 13;
                    size_t difficultyEnd = request.body.find(",", difficultyStart);
                    if (difficultyEnd == std::string::npos) {
                        difficultyEnd = request.body.find("}", difficultyStart);
                    }
                    std::string difficultyStr = request.body.substr(difficultyStart, difficultyEnd - difficultyStart);
                    int difficulty = std::stoi(difficultyStr);
                    
                    // Validate difficulty
                    if (difficulty < 1 || difficulty > 5) {
                        response = badRequest("Difficulty must be between 1 and 5");
                    } else {
                        // Extract description (optional)
                        std::string description;
                        size_t descriptionPos = request.body.find("\"description\":");
                        if (descriptionPos != std::string::npos) {
                            size_t descriptionStart = request.body.find("\"", descriptionPos + 14) + 1;
                            size_t descriptionEnd = request.body.find("\"", descriptionStart);
                            description = request.body.substr(descriptionStart, descriptionEnd - descriptionStart);
                        }
                        
                        // Extract due date (optional)
                        std::string dueDate;
                        size_t dueDatePos = request.body.find("\"dueDate\":");
                        if (dueDatePos != std::string::npos) {
                            size_t dueDateStart = request.body.find("\"", dueDatePos + 10) + 1;
                            size_t dueDateEnd = request.body.find("\"", dueDateStart);
                            dueDate = request.body.substr(dueDateStart, dueDateEnd - dueDateStart);
                            
                            // Validate date format
                            if (!dueDate.empty() && (dueDate.length() != 10 || dueDate[4] != '-' || dueDate[7] != '-')) {
                                response = badRequest("Due date must be in YYYY-MM-DD format");
                                dueDate = "";
                            }
                        }
                        
                        if (response.empty()) {
                            todoList.addTask(header, description, difficulty, dueDate);
                            response = created();
                        }
                    }
                }
            } catch (const std::exception& e) {
                response = badRequest(e.what());
            }
        }
        // Mark task as complete
        else if (request.path.rfind("/api/tasks/", 0) == 0 && request.method == "POST") {
            std::string pathParam = getPathParam(request.path, "/api/tasks/");
            size_t slashPos = pathParam.find('/');
            
            if (slashPos != std::string::npos) {
                std::string taskIdStr = pathParam.substr(0, slashPos);
                std::string action = pathParam.substr(slashPos + 1);
                
                int taskId = extractTaskId(taskIdStr);
                if (taskId < 0) {
                    response = badRequest("Invalid task ID");
                } else {
                    if (action == "complete") {
                        bool marked = todoList.markTaskAsCompleted(taskId);
                        response = marked ? okJson("{}") : notFound("Task not found");
                    } else if (action == "uncomplete") {
                        bool unmarked = todoList.unmarkTaskAsCompleted(taskId);
                        response = unmarked ? okJson("{}") : notFound("Task not found");
                    } else {
                        response = notFound("Unknown action");
                    }
                }
            } else {
                response = badRequest("Invalid path");
            }
        }
        // Delete a task
        else if (request.path.rfind("/api/tasks/", 0) == 0 && request.method == "DELETE") {
            std::string pathParam = getPathParam(request.path, "/api/tasks/");
            int taskId = extractTaskId(pathParam);
            
            if (taskId < 0) {
                response = badRequest("Invalid task ID");
            } else {
                try {
                    todoList.deleteTask(taskId);
                    response = noContent();
                } catch (const std::exception& e) {
                    response = badRequest(e.what());
                }
            }
        }
        // Update a task
        else if (request.path.rfind("/api/tasks/", 0) == 0 && request.method == "PUT") {
            std::string pathParam = getPathParam(request.path, "/api/tasks/");
            int taskId = extractTaskId(pathParam);
            
            if (taskId < 0) {
                response = badRequest("Invalid task ID");
            } else {
                try {
                    // Parse JSON body - simplified for brevity
                    size_t headerPos = request.body.find("\"header\":");
                    size_t difficultyPos = request.body.find("\"difficulty\":");
                    
                    if (headerPos == std::string::npos || difficultyPos == std::string::npos) {
                        response = badRequest("Missing required fields");
                    } else {
                        // Extract header
                        size_t headerStart = request.body.find("\"", headerPos + 9) + 1;
                        size_t headerEnd = request.body.find("\"", headerStart);
                        std::string header = request.body.substr(headerStart, headerEnd - headerStart);
                        
                        // Extract difficulty
                        size_t difficultyStart = difficultyPos + 13;
                        size_t difficultyEnd = request.body.find(",", difficultyStart);
                        if (difficultyEnd == std::string::npos) {
                            difficultyEnd = request.body.find("}", difficultyStart);
                        }
                        std::string difficultyStr = request.body.substr(difficultyStart, difficultyEnd - difficultyStart);
                        int difficulty = std::stoi(difficultyStr);
                        
                        // Validate difficulty
                        if (difficulty < 1 || difficulty > 5) {
                            response = badRequest("Difficulty must be between 1 and 5");
                        } else {
                            // Extract description (optional)
                            std::string description;
                            size_t descriptionPos = request.body.find("\"description\":");
                            if (descriptionPos != std::string::npos) {
                                size_t descriptionStart = request.body.find("\"", descriptionPos + 14) + 1;
                                size_t descriptionEnd = request.body.find("\"", descriptionStart);
                                description = request.body.substr(descriptionStart, descriptionEnd - descriptionStart);
                            }
                            
                            // Extract due date (optional)
                            std::string dueDate;
                            size_t dueDatePos = request.body.find("\"dueDate\":");
                            if (dueDatePos != std::string::npos) {
                                size_t dueDateStart = request.body.find("\"", dueDatePos + 10) + 1;
                                size_t dueDateEnd = request.body.find("\"", dueDateStart);
                                dueDate = request.body.substr(dueDateStart, dueDateEnd - dueDateStart);
                                
                                // Validate date format
                                if (!dueDate.empty() && (dueDate.length() != 10 || dueDate[4] != '-' || dueDate[7] != '-')) {
                                    response = badRequest("Due date must be in YYYY-MM-DD format");
                                    dueDate = "";
                                }
                            }
                            
                            if (response.empty()) {
                                todoList.editTask(taskId, header, description, difficulty, dueDate);
                                response = okJson("{}");
                            }
                        }
                    }
                } catch (const std::exception& e) {
                    response = badRequest(e.what());
                }
            }
        }
        // Catch-all for unknown routes
        else {
            response = notFound("Endpoint not found");
        }
        
        // Send response
        write(clientSocket, response.c_str(), response.length());
        
        // Close connection
        close(clientSocket);
    }
    
    // Cleanup
    close(serverSocket);
    
    return 0;
} 