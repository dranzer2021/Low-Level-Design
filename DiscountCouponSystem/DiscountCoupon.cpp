#include <vector>
#include <string>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

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

class Cart {
    vector<CartItem*> items;
    bool loyaltyMember;
    double originalTotal;
    double currentTotal;
    string paymentBank;

    public:
    
    Cart() {
        originalTotal = 0.0;
        currentTotal = 0.0;
        loyaltyMember = false;
        paymentBank = "";
    }
    void addProduct(Product* product,int quantity) {
        CartItem* item = new CartItem(product,quantity);
        items.push_back(item);
        originalTotal += item->itemTotal();
        currentTotal  += item->itemTotal();
    }
    void applyDiscount(double amount) {
        currentTotal -= amount;
        if (currentTotal < 0) {
            currentTotal = 0;
        }
    }
    double getOriginalTotal() {
        return originalTotal;
    }
    double getCurrentTotal() {
        return currentTotal;
    }
    string getPaymentBank() {
        return paymentBank;
    }
    vector<CartItem*> getItems() {
        return items;
    }
    bool isLoyaltyMember() {
        return loyaltyMember;
    }

    void setLoyaltyMember(bool member) {
        loyaltyMember = member;
    }
    void setPaymentBank(string bank) {
        paymentBank = bank;
    }
    
};

class Coupon {
    Coupon* next;

    public:
    Coupon() {
        next = nullptr;
    }
    virtual ~Coupon() {
        if (next) {
            delete next;
        }
    }
    Coupon* getNext() {
        return next;
    }
    void setNext(Coupon* nxt) {
        next = nxt;
    }
     void applyDiscount(Cart* cart) {
        if (isApplicable(cart)) {
            double discount = getDiscount(cart);
            cart->applyDiscount(discount);
            cout << name() << " applied: " << discount << endl;
            if (!isCombinable()) {
                return;
            }
        }
        if (next) {
            next->applyDiscount(cart);
        }
    }
    virtual bool isApplicable(Cart* cart) = 0;
    virtual double getDiscount(Cart* cart) = 0;
    virtual bool isCombinable() {
        return true;
    }
    virtual string name() = 0;
};

class SeasonalOffer : public Coupon {
    double percent;
    string category;
    DiscountStrategy* ds;

    public:
    SeasonalOffer(double pct,string cat) {
        percent = pct;
        category = cat;
        ds = DiscountStrategyManager::getInstance()->getStrategy(StrategyType::PERCENT,percent);
    }
    ~SeasonalOffer() {
        delete ds;
    }
    bool isApplicable(Cart* cart) override {
        for(CartItem* item:cart->getItems()) {
            if(item->getProduct()->getCategory() == category){
                return true;
            }
        }
        return false;
    }
    double getDiscount(Cart* cart) override {
        double subtotal = 0.0;
        for(CartItem* item:cart->getItems()) {
            if(item->getProduct()->getCategory() == category) {
                subtotal += item->itemTotal();
            }
            return ds->calculate(subtotal);
        }
    }
    bool isCombinable() override {
        return true;
    }
    string name() override {
        return "Seasonal Offer " + to_string((int)percent) + " percent off " + category;
    }
};

class LoyaltyDiscount : public Coupon {
    double percent;
    DiscountStrategy* ds;

    public:
    LoyaltyDiscount(double pct) {
        percent = pct;
        ds = DiscountStrategyManager::getInstance()->getStrategy(StrategyType::PERCENT,percent);
    }
    ~LoyaltyDiscount() {
        delete ds;
    }
    bool isApplicable(Cart* cart) override {
        return cart->isLoyaltyMember();
    }
    double getDiscount(Cart* cart) override {
        return ds->calculate(cart->getCurrentTotal());
    }
    string name() override {
        return "Loyalty Discount " + to_string((int)percent) + " percent off ";
    }
};

class BulkPurchaseDiscount : public Coupon {
    double threshold;
    double flatoff;
    DiscountStrategy* ds;

    public:
    BulkPurchaseDiscount(double thr, double off) {
        threshold = thr;
        flatoff = off;
        ds = DiscountStrategyManager::getInstance()->getStrategy(StrategyType::FLAT,flatoff);
    }
    ~BulkPurchaseDiscount() {
        delete ds;
    }
    bool isApplicable(Cart* cart) override {
        return cart->getOriginalTotal() >= threshold;
    }
    double getDiscount(Cart* cart) override {
        return ds->calculate(cart->getCurrentTotal());
    }
    string name() override {
        return "Bulk Purchase Rs " + to_string((int)flatoff) + " off over "
             + to_string((int)threshold);
    }
};

class BankingCoupon : public Coupon {
    string bank;
    double minSpend;
    double percent;
    double offCap;
    DiscountStrategy* strat;
    
    public:
    BankingCoupon(const string& b, double ms, double percent, double offCap) {
        bank = b;
        minSpend = ms;
        this->percent = percent;
        this->offCap = percent;
        strat = DiscountStrategyManager::getInstance()->getStrategy(StrategyType::PERCENT_WITH_CAP, percent, offCap);
    }
    ~BankingCoupon() {
        delete strat;
    }
    bool isApplicable(Cart* cart) override {
        return cart->getPaymentBank() == bank
            && cart->getOriginalTotal() >= minSpend;
    }
    double getDiscount(Cart* cart) override {
        return strat->calculate(cart->getCurrentTotal());
    }
    string name() override {
        return bank + " Bank Rs " + to_string((int)percent) + " off upto " + to_string((int) offCap);
    }
};

class CouponManager {
    Coupon* head;
    static CouponManager* instance;
    mutable mutex mtx;
    CouponManager() {
        head = nullptr;
    }
    public:

    static CouponManager* getInstance() {
        if(!instance) {
            instance = new CouponManager();
        }
        return instance;
    }
    void registerCoupon(Coupon* coupon) {
        lock_guard<mutex> lock(mtx);
        if(!head) {
            head = coupon;
        }
        else {
            Coupon* cur = head;
            while(cur->getNext()) {
                cur = cur->getNext();
            }
            cur->setNext(coupon);
        }
    }
    vector<string> getApplicable(Cart* cart) const {
        lock_guard<mutex> lock(mtx);
        vector<string> res;
        Coupon* cur = head;
        while(cur) {
            if(cur->isApplicable(cart)) {
                res.push_back(cur->name());
            }
            cur = cur->getNext();
        }
        return res;
    }

    double applyAll(Cart* cart) {
        lock_guard<mutex> lock(mtx);
        if(head) {
            head->applyDiscount(cart);
        }
        return cart->getCurrentTotal();
    }
    
};
CouponManager* CouponManager::instance = nullptr;

int main() {

    CouponManager* mgr = CouponManager::getInstance();
    mgr->registerCoupon(new SeasonalOffer(10, "Clothing"));
    mgr->registerCoupon(new LoyaltyDiscount(5));
    mgr->registerCoupon(new BulkPurchaseDiscount(1000, 100));
    mgr->registerCoupon(new BankingCoupon("ABC", 2000, 15, 500));

    Product* p1 = new Product("Winter Jacket", "Clothing", 1000);
    Product* p2 = new Product("Smartphone", "Electronics", 20000);
    Product* p3 = new Product("Jeans", "Clothing", 1000);
    Product* p4 = new Product("Headphones", "Electronics", 2000);

    Cart* cart = new Cart();
    cart->addProduct(p1, 1);
    cart->addProduct(p2, 1);
    cart->addProduct(p3, 2);
    cart->addProduct(p4, 1);
    cart->setLoyaltyMember(true);
    cart->setPaymentBank("ABC");

    cout << "Original Cart Total: Rs." << cart->getOriginalTotal() << endl;

    vector<string> applicable = mgr->getApplicable(cart);
    cout << "Applicable Coupons:" << endl;
    for (string name : applicable) {
        cout << " - " << name << endl;
    }

    double finalTotal = mgr->applyAll(cart);
    cout << "Final Cart Total after discounts: " << finalTotal << " Rs" << endl;

    // Cleanup code
    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete cart;
    return 0;
}