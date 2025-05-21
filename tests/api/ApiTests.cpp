#include <gtest/gtest.h>
#include <curl/curl.h>
#include <string>
#include <iostream>

// Callback function to write received data from Curl
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
        return newLength;
    } catch(std::bad_alloc &e) {
        return 0;
    }
}

class ApiTest : public ::testing::Test {
protected:
    void SetUp() override {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    void TearDown() override {
        curl_global_cleanup();
    }
    
    // Helper function to perform GET request
    std::string performGet(const std::string& url) {
        CURL* curl = curl_easy_init();
        std::string readBuffer;
        
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            
            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            }
            
            curl_easy_cleanup(curl);
        }
        
        return readBuffer;
    }
};

// Test the health endpoint
TEST_F(ApiTest, HealthEndpoint) {
    std::string response = performGet("http://localhost:8080/health");
    EXPECT_FALSE(response.empty());
    EXPECT_NE(response.find("ok"), std::string::npos);
}

// Test getting all tasks
TEST_F(ApiTest, GetAllTasks) {
    std::string response = performGet("http://localhost:8080/api/tasks");
    EXPECT_FALSE(response.empty());
    // Further assertions would depend on expected response format
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 