#ifndef CREDIT_CARD_PAYMENT_STRATEGY_H
#define CREDIT_CARD_PAYMENT_STRATEGY_H

#include <iostream>
#include <string>
#include "PaymentStrategy.h"
using namespace std;

class CreditCardPaymentStrategy : public PaymentStrategy{
    string cardNumber;
    public:
    CreditCardPaymentStrategy(const string& card) {
        cardNumber = card;
    }
    void pay(double amount) override {
        cout<<"Paid Rs."<<amount<<" using Credit Card ("<<cardNumber<<")"<<endl;
    }
};

#endif