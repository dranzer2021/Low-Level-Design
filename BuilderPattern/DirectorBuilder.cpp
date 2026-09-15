#include<bits/stdc++.h>
using namespace std;

class HttpRequest {
    private:
    string url;
    string method;
    map<string,string> headers;
    map<string,string> queryparams;
    string body;
    int timeout;

    HttpRequest() { }

    public:
    friend class HttpRequestBuilder;

    void execute(){
        cout<<"Url: "<<url<<endl;
        cout<<"Method: "<<method<<endl;
        if(!queryparams.empty()){
            cout<<"Query Parameters:"<<endl;
            for(auto it:queryparams){
                cout<<" "<<it.first<<"="<<it.second<<endl;
            }
        }
        cout<<"Headers:"<<endl;
        for(auto it:headers){
            cout<<" "<<it.first<<" : "<<it.second<<endl;
        }

        cout<<"Body: "<<body<<endl;
        cout<<"Timeout: "<<timeout<<" seconds"<<endl;
        cout<<"Request Executed Successfully! "<<endl;
    }
};

class HttpRequestBuilder{
    private:
    HttpRequest req;

    public:
    HttpRequestBuilder& withUrl(const string& u){
        req.url=u;
        return *this;
    }
    HttpRequestBuilder& withMethod(const string& m){
        req.method=m;
        return *this;
    }
    HttpRequestBuilder& withQueryParams(const string& key , const string& value){
        req.queryparams[key]=value;
        return *this;
    }
    HttpRequestBuilder& withHeaders(const string& key,const string& value){
        req.headers[key]=value;
        return *this;
    }
    HttpRequestBuilder& withBody(const string& b){
        req.body=b;
        return *this;
    }
    HttpRequestBuilder& withTimeout(int t){
        req.timeout=t;
        return *this;
    }
    HttpRequest build(){
        if(req.url.empty()){
            throw runtime_error("URL can't be empty");
        }
        return req;
    }
};

class HttpRequestDirector {
    public:
    static HttpRequest createGetRequest(const string& url){
        return HttpRequestBuilder()
            .withUrl(url)
            .withMethod("GET")
            .build();
    }

    static HttpRequest createPostRequest(const string& url,const string& jsonBody){
        return HttpRequestBuilder()
            .withUrl(url)
            .withMethod("POST")
            .withBody(jsonBody)
            .build();
    }
};

int main(){
    
    HttpRequest request = HttpRequestBuilder()
        .withUrl("https://www.google.co.in")
        .withMethod("GET")
        .withQueryParams("Key","12345")
        .withHeaders("Content-Type","JSON")
        .withBody("Sample Request")
        .withTimeout(60)
        .build();

    request.execute();
    cout <<"\n----------------------------\n";
    HttpRequest getRequest = HttpRequestDirector::createGetRequest("https://api.example.com");
    getRequest.execute();
    cout <<"\n----------------------------\n";
    HttpRequest postRequest = HttpRequestDirector::createPostRequest("https://www.gmail.com","Gmail Inbox");
    postRequest.execute();

}