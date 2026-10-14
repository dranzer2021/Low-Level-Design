#include <vector>
#include <string>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


class Product{
    string name;
    string category;
    double price;
    public:
    Product(string n, string c, double p) {
        name = n;
        category = c;
        price = p;
    }
    string getName() {
        return name;
    }
    string getCategory() const {
        return category;
    }
    double getPrice() {
        return price;
    }
};

class CartItem {
    Product* product;
    int quantity;

    public:
    CartItem(Product* prod, int qty) {
        product = prod;
        quantity = qty;
    }
    double itemTotal() {
        return product->getPrice() * quantity;
    }
    const Product* getProduct() {
        return product;
    }
    
};

class DiscountStrategy {
    public:
    virtual ~DiscountStrategy() {}
    virtual double calculate(double baseAmount) = 0; 
};

class FlatDiscountStrategy : public DiscountStrategy {
    double amount;
    public:
    FlatDiscountStrategy(double amt) {
        amount = amt;
    }
    double calculate(double baseAmount) override {
        return min(amount, baseAmount);
    }
};

class PercentageDiscountStrategy : public DiscountStrategy {
    double percent;
    public:
    PercentageDiscountStrategy(double pct) {
        percent = pct;
    }
    double calculate(double baseAmount) override {
        return (percent / 100.0) * baseAmount;
    }
};

class PercentageWithCapStrategy : public DiscountStrategy {
private:
    double percent;
    double cap;
public:
    PercentageWithCapStrategy(double pct, double capVal) {
        percent = pct;
        cap = capVal;
    }
    double calculate(double baseAmount) override {
        double disc = (percent / 100.0) * baseAmount;
        if (disc > cap) {
            return cap;
        }
        return disc;
    }
};

enum class StrategyType {
    FLAT,
    PERCENT,
    PERCENT_WITH_CAP
};

class DiscountStrategyManager {
    static DiscountStrategyManager* instance;
    DiscountStrategyManager () {

    }
    public:
    static DiscountStrategyManager* getInstance() {
        if(!instance) {
            instance = new DiscountStrategyManager();
        }
        return instance;
    }
    ~DiscountStrategyManager() {}

    DiscountStrategy* getStrategy(StrategyType type, double param1, double param2 = 0.0) const {
        if (type == StrategyType::FLAT) {
            return new FlatDiscountStrategy(param1);
        }
        if (type == StrategyType::PERCENT) {
            return new PercentageDiscountStrategy(param1);
        }
        if (type == StrategyType::PERCENT_WITH_CAP) {
            return new PercentageWithCapStrategy(param1, param2);
        }
        return nullptr;
    }

};
DiscountStrategyManager* DiscountStrategyManager::instance = nullptr;


int main() {

    return 0;
}