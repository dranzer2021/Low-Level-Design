#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct PaymentRequest {
    public:
    string sender;
    string receiver;
    double amount;
    string currency;
    PaymentRequest(const string& s,const string& r,double amt,const string& curr) {
        this->sender = s;
        this->receiver = r;
        this->amount = amt;
        this->currency = curr;
    }
};

class BankingSystem {
    public:
    virtual bool processPayment(double amount) = 0;
    ~BankingSystem() {}
};
class PaytmBankingSystem : public BankingSystem {
    public:
    PaytmBankingSystem() {}
    bool processPayment(double amount) override {
        cout<<"[BankingSystem : Paytm] Processing Payment of Rs."<<amount<<" ...\n";
        int r = rand() % 100;
        return r<80;
    }
};
class RazorpayBankingSystem : public BankingSystem {
    public:
    RazorpayBankingSystem() {}
    bool processPayment(double amount) override {
        cout<<"[BankingSystem : Razorpay] Processing Payment of Rs."<<amount<<" ...\n";
        int r = rand() % 100;
        return r<90;
    }
};

class PaymentGateway {
    protected:
    BankingSystem* bankingSystem;
    public:
    PaymentGateway() { 
        bankingSystem = nullptr;
    }
    virtual ~PaymentGateway() { 
        delete bankingSystem; 
    }
    virtual bool processPayment(PaymentRequest* request) {
        if(!validate(request)) {
            cout<<"[Payment Gateway] Validation failed for :"<<request->sender<<"\n";
            return false;
        }
        if(!initiate(request)) {
            cout<<"[Payment Gateway] Initiation failed for :"<<request->sender<<"\n";
            return false;
        }
        if(!confirm(request)) {
            cout<<"[Payment Gateway] Confirmation failed for :"<<request->sender<<"\n";
            return false;
        }
        return true;
    }
    virtual bool validate(PaymentRequest* request) = 0;
    virtual bool initiate(PaymentRequest* request) = 0;
    virtual bool confirm(PaymentRequest* request) = 0;
};
class PaytmGateway : public PaymentGateway {
    public:
    PaytmGateway() {
        bankingSystem = new PaytmBankingSystem();
    }
    bool validate(PaymentRequest* request) override {
        cout<<"[Paytm] Validating Payment for "<<request->sender<<". \n";
        if(request->amount <= 0 || request->currency != "INR") {
            return false;
        }
        return true;
    }
    bool initiate(PaymentRequest* request) override {
        cout<<"[Paytm] Initiating Payment for "<<request->sender<<" of Rs."<<request->amount<<". \n";
        return bankingSystem->processPayment(request->amount);
    }
    bool confirm(PaymentRequest* request) override {
        cout << "[Paytm] Confirming payment for " << request->sender << ".\n";
        return true;
    }
};
class RazorpayGateway : public PaymentGateway {
    public:
    RazorpayGateway() {
        bankingSystem = new RazorpayBankingSystem();
    }
    bool validate(PaymentRequest* request) override {
        cout<<"[Razorpay] Validating Payment for "<<request->sender<<". \n";
        if(request->amount <= 0 || request->currency != "INR") {
            return false;
        }
        return true;
    }
    bool initiate(PaymentRequest* request) override {
        cout<<"[Razorpay] Initiating Payment for "<<request->sender<<" of Rs."<<request->amount<<". \n";
        return bankingSystem->processPayment(request->amount);
    }
    bool confirm(PaymentRequest* request) override {
        cout << "[Razorpay] Confirming payment for " << request->sender << ".\n";
        return true;
    }
};
class PaymentGatewayProxy : public PaymentGateway {
    PaymentGateway* realGateway;
    int retries;
    public:
    PaymentGatewayProxy(PaymentGateway* gateway,int maxRetries) {
        retries = maxRetries;
        realGateway = gateway;
    }
    ~PaymentGatewayProxy() {
        delete realGateway;
    }
    bool processPayment(PaymentRequest* request) override {
        bool result = false;
        for (int attempt = 0; attempt < retries; ++attempt) {
            if (attempt > 0) {
                cout << "[Proxy] Retrying payment (attempt " << (attempt+1)
                          << ") for " << request->sender << ".\n";
            }
            result = realGateway->processPayment(request);
            if (result) break;
        }
        if (!result) {
            cout << "[Proxy] Payment failed after " << (retries)
                      << " attempts for " << request->sender << ".\n";
        }
        return result;
    }
    bool validate(PaymentRequest* request) override {
        return realGateway->validate(request);
    }
    bool initiate(PaymentRequest* request) override {
        return realGateway->initiate(request);
    }
    bool confirm(PaymentRequest* request) override {
        return realGateway->confirm(request);
    }
};

enum class GatewayType {
    PAYTM,
    RAZORPAY
};

class GatewayFactory {
    static GatewayFactory instance;
    GatewayFactory() {}
    GatewayFactory(const GatewayFactory&) = delete;
    GatewayFactory& operator = (const GatewayFactory&) = delete;

    public:
    static GatewayFactory& getInstance() {
        return instance;
    }
    PaymentGateway* getGateway(GatewayType type) {
        if(type == GatewayType::PAYTM) {
            PaymentGateway* paymentGateway = new PaytmGateway();
            return new PaymentGatewayProxy(paymentGateway,3);
        }
        else {
            PaymentGateway* paymentGateway = new RazorpayGateway();
            return new PaymentGatewayProxy(paymentGateway,1);
        }
    }
};
GatewayFactory GatewayFactory::instance;

class PaymentService {
    static PaymentService instance;
    PaymentGateway* gateway;
    PaymentService() {
        gateway = nullptr;
    }
    ~PaymentService() {
        delete gateway;
    }
    PaymentService(const PaymentService&) = delete;
    PaymentService& operator = (const PaymentService&) = delete;

    public:
    static PaymentService& getInstance() {
        return instance;
    }
    void setGateway(PaymentGateway* g) {
        if(gateway) delete gateway;
        gateway = g;
    }
    bool processPayment(PaymentRequest* request) {
        if(!gateway) {
            cout<<"[PaymentService] No payment gateway selected.\n";
            return false;
        }
        return gateway->processPayment(request);
    }
};
PaymentService PaymentService::instance;

class PaymentController {

    static PaymentController instance;
    PaymentController() {}
    PaymentController(const PaymentController&) = delete;
    PaymentController& operator=(const PaymentController&) = delete;

public:
    static PaymentController& getInstance() {
        return instance;
    }
    bool handlePayment(GatewayType type, PaymentRequest* req) {
        PaymentGateway* paymentGateway = GatewayFactory::getInstance().getGateway(type);
        PaymentService::getInstance().setGateway(paymentGateway);
        return PaymentService::getInstance().processPayment(req);
    }
};
PaymentController PaymentController::instance;

int main () {
    
    srand(static_cast<unsigned>(time(nullptr)));
    
    PaymentRequest* req1 = new PaymentRequest("Aditya", "Shubham", 1000.0, "INR");
    cout << "Processing via Paytm\n";
    cout << "------------------------------\n";
    bool res1 = PaymentController::getInstance().handlePayment(GatewayType::PAYTM, req1);
    cout << "Result: " << (res1 ? "SUCCESS" : "FAIL") << "\n";
    cout << "------------------------------\n\n";

    PaymentRequest* req2 = new PaymentRequest("Shubham", "Aditya", 500.0, "USD");

    cout << "Processing via Razorpay\n";
    cout << "------------------------------\n";
    bool res2 = PaymentController::getInstance().handlePayment(GatewayType::RAZORPAY, req2);
    cout << "Result: " << (res2 ? "SUCCESS" : "FAIL") << "\n";
    cout << "------------------------------\n";

    return 0;
}