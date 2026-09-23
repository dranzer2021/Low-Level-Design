#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <iostream>
#include <string>
#include <vector>
#include <MenuItem.h>
using namespace std;

class Restaurant
{
private:
    static int nextRestaurantId;
    int restaurantId;
    string name;
    string loc;
    vector<MenuItem> menu;
public:
    Restaurant(const string& name, const string& loc){
        this->name = name;
        this->loc = loc;
        this->restaurantId = ++nextRestaurantId;
    }
    ~Restaurant(){
        cout<<"Destroying Restaurant: "<<name<<" and clearing its menu." <<endl;
        menu.clear();
    }
    string getName() {
        return name;
    }
    string getLoc() {
        return loc;
    }
    const vector<MenuItem>& getMenu() const {
        return menu;
    }
    void setName(const string& n) {
        name = n;
    }
    void setLoc(const string& l) {
        loc = l;
    }
    void addMenuItem(const MenuItem &item) {
        menu.push_back(item);
    }
};
int Restaurant::nextRestaurantId = 0;

#endif