#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include "Cart.h"
using namespace std;

class User {
    int userId;
    string name;
    string address;
    Cart* cart;

    public:
    User(int userId, const string& name, const string& address){
        this->userId = userId;
        this->name = name;
        this->address = address;
        cart = new Cart();
    }
    ~User(){
        delete cart;
    }
    string getName() {
        return name;
    }
    string getAddress() {
        return address;
    }
    void setName(const string& n){
        name = n;
    }
    void setAddress(const string& a){
        address = a;
    }
    Cart* getCart() const {
        return cart;
    }
};

#endif