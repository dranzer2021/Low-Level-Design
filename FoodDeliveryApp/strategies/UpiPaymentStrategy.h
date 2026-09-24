#ifndef UPI_PAYMENT_STRATEGY_H
#define UPI_PAYMENT_STRATEGY_H

#include <iostream>
#include <string>
#include "PaymentStrategy.h"
using namespace std;

class UpiPaymentStrategy : public PaymentStrategy {
    string mobile;
    public:
    UpiPaymentStrategy(const string& mob){
        mobile = mob;
    }
    void pay(double amount) override {
        cout<<"Paid Rs."<<amount<<" using UPI ("<<mobile<<")"<<endl;
    }
};

#endif