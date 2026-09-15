#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

using namespace std;

class HttpRequest {
private:
    string url;
    string method;
    map<string, string> headers;
    map<string,string> queryParams;
    string body;
    int timeout; // in seconds

    // Private constructor - can only be accessed by the Builder
    HttpRequest() { }

public:
    friend class HttpRequestBuilder;

    // Method to execute the HTTP request
    void execute() {
        cout << "Executing " << method << " request to " << url << endl;
        
        if (!queryParams.empty()) {
            cout << "Query Parameters:" << endl;
            for (const auto& param : queryParams) {
                cout << "  " << param.first << "=" << param.second << endl;
            }
        }

        cout << "Headers:" << endl;
        for (const auto& header : headers) {
            cout << "  " << header.first << ": " << header.second << endl;
        }
        
        if (!body.empty()) {
            cout << "Body: " << body << endl;
        }
        
        cout << "Timeout: " << timeout << " seconds" << endl;
        cout << "Request executed successfully!" << endl;
    }
};

class MethodStep;
class HeaderStep;
class OptionalStep;

class UrlStep{
    public:
    virtual MethodStep& withUrl(const string& u) = 0;
};

class MethodStep{
    public:
    virtual HeaderStep& withMethod(string method) = 0;
};

class HeaderStep{
    public:
    virtual OptionalStep& withHeader(const string& key, const string& value) = 0;
};

class OptionalStep{
    public:
    virtual ~OptionalStep() {}
    virtual OptionalStep& withBody(const string& body) = 0;
    virtual OptionalStep& withTimeout(int timeout) = 0;
    virtual HttpRequest build() = 0;
};

class HttpRequestBuilder:
    public UrlStep,
    public MethodStep,
    public HeaderStep,
    public OptionalStep {

private:
    HttpRequest req;

public:    
    // Method chaining
    MethodStep& withUrl(const string& u) override {
        req.url = u;
        return *this;
    }

    HeaderStep& withMethod(string method) override {
        req.method = method;
        return *this;
    }
    
    OptionalStep& withHeader(const string& key, const string& value) override {
        req.headers[key] = value;
        return *this;
    }
    
    OptionalStep& withBody(const string& body) override {
        req.body = body;
        return *this;
    }
    
    OptionalStep& withTimeout(int timeout) override {
        req.timeout = timeout;
        return *this;
    }
    
    // Build method to create the immutable HttpRequest object
    HttpRequest build() override {
        // Validation logic can be added here
        if (req.url.empty()) {
            throw runtime_error("URL cannot be empty");
        }
        return req;
    }
    static UrlStep& getBuilder(){
        return *(new HttpRequestBuilder());
    }
};

int main() {

    HttpRequest stepRequest = HttpRequestBuilder::getBuilder()
        .withUrl("https://api.example.com")
        .withMethod("POST")
        .withHeader("Content-Type", "application/json")
        .withBody("{\"name\": \"Aditya\"}")
        .withTimeout(60)
        .build();
    
        stepRequest.execute(); 

    cout <<"\n----------------------------\n";
}