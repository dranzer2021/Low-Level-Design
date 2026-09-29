#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class PaymentRequest {
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
    BankingSystem* bankingSystem;
    public:
    bool processPayment(PaymentRequest* request) {
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

};
class RazorpayGateway : public PaymentGateway {

};

int main () {
    
    return 0;
}