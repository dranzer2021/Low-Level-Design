#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <string>
using namespace std;

class MenuItem {
    private:
    string code;
    string name;
    int price;

    public:
    MenuItem(const string& code, const string& name, int price) {
        this->code = code;
        this->name = name;
        this->price = price;
    }

    string getCode() {
        return code;
    }

    void setCode(const string& c) {
        code = c;
    }

    string getName() {
        return name;
    }

    void setName(const string& n) {
        name = n;
    }

    int getPrice() {
        return price;
    }

    void setPrice(int p){
        price = p;
    }
};

#endif