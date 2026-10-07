#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

class Product {
    int sku;
    string name;
    double price;

    public:
    Product(int id,string nm,double pr) {
        sku = id;
        name = nm;
        price = pr;
    }

    int getSku() {
        return this->sku;
    }
    string getName() {
        return this->name;
    }
    double getPrice() {
        return this->price;
    }
};

class ProductFactory {
    public:
    static Product* createProduct(int sku) {
        string name;
        double price;
        if (sku == 101) {
            name  = "Apple";
            price = 20;
        }
        else if (sku == 102) {
            name  = "Banana";
            price = 10;
        }
        else if (sku == 103) {
            name  = "Chocolate";
            price = 50;
        }
        else if (sku == 201) {
            name  = "T-Shirt";
            price = 500;
        }
        else if (sku == 202) {
            name  = "Jeans";
            price = 1000;
        }
        else {
            name  = "Item" + to_string(sku);
            price = 100;
        }
        return new Product(sku, name, price);
    }
};

class InventoryStore {
    public:
    virtual ~InventoryStore() {}
    virtual void addProduct(Product* prod,int qty) = 0;
    virtual void removeProduct(int sku,int qty) = 0;
    virtual int checkStock(int sku) = 0;
    virtual vector<Product*> listAvailableProducts() = 0;
};
class DbInventoryStore : public InventoryStore {
    map<int,int>* stock;
    map<int,Product*>* products;
    public:
    DbInventoryStore() {
        stock = new map<int,int>();
        products = new map<int,Product*>();
    }
    ~DbInventoryStore() {
        for(auto it:*products) {
            delete it.second;
        }
        delete products;
        delete stock;
    }
    void addProduct(Product* prod,int qty) override {
        int sku = prod->getSku();
        if(products->count(sku) == 0) {
            (*products)[sku] = prod;
        }
        else {
            delete prod;
        }
        (*stock)[sku] += qty;
    }
    void removeProduct(int sku,int qty) override {
        if (stock->count(sku) == 0) 
            return;
        int remainingQty = (*stock)[sku] - qty;
        if(remainingQty>0) {
            (*stock)[sku] = remainingQty; 
        }
        else{
            stock->erase(sku);
        }
    }
    int checkStock(int sku) override {
        if(stock->count(sku) == 0 ) return 0;
        int qty = (*stock)[sku];
        return qty;
    }
    vector<Product*> listAvailableProducts() override {
        vector<Product*> listOfAvailableProducts;
        for(auto it:*stock) {
            int sku = it.first;
            int qty = it.second;
            if(qty > 0 && products->count(sku)){
                listOfAvailableProducts.push_back((*products)[sku]);
            }
        }
        return listOfAvailableProducts;
    }
};

class InventoryManager {
    InventoryStore* store;
    public:
    InventoryManager(InventoryStore* store) {
        this->store = store;
    }
    void addStock(int sku,int qty) {
        Product* product = ProductFactory::createProduct(sku);
        store->addProduct(product,qty);
        cout << "[InventoryManager] Added SKU " << sku << " Qty " << qty << endl;
    }
    void removeStock(int sku,int qty) {
        store->removeProduct(sku,qty);
    }
    int checkStock(int sku) {
        return store->checkStock(sku);
    }
    vector<Product*> getAvailableProducts() {
        return store->listAvailableProducts();
    }
};

class ReplenishStrategy {
    public:
    virtual void replenish(InventoryManager* manager,map<int,int> itemsToReplenish) = 0;
    virtual ~ReplenishStrategy() {}
};
class ThresholdReplenishStrategy : public ReplenishStrategy {
    int threshold;

    public:
    ThresholdReplenishStrategy(int threshold) {
        this->threshold = threshold;
    }
    void replenish(InventoryManager* manager,map<int,int> itemsToReplenish) override {
        cout << "[ThresholdReplenish] Checking threshold... \n";
        for (auto it : itemsToReplenish) {
            int sku = it.first;
            int qtyToAdd = it.second;
            int current  = manager->checkStock(sku);
            if (current < threshold) {
                manager->addStock(sku, qtyToAdd);
                cout << "  -> SKU " << sku << " was " << current 
                     << ", replenished by " << qtyToAdd << endl;
            }
        }
    }
};
class WeeklyReplenishStrategy : public ReplenishStrategy {
public:
    WeeklyReplenishStrategy() {}
    void replenish(InventoryManager* manager, map<int,int> itemsToReplenish) override {
        cout << "[WeeklyReplenish] Weekly replenishment triggered for inventory.\n";
    }
};

class DarkStore {
    string name;
    double x,y;
    InventoryManager* inventoryManager;
    ReplenishStrategy* replenishStrategy;
    
    public:
    DarkStore(string n, double x_coord, double y_coord) {
        name = n;
        x = x_coord;
        y = y_coord;

        inventoryManager = new InventoryManager(new DbInventoryStore);
    }

    ~DarkStore() {
        delete inventoryManager;
        if (replenishStrategy) delete replenishStrategy;
    }
    double distanceTo(double ux, double uy) {
        return sqrt((x - ux)*(x - ux) + (y - uy)*(y - uy));
    }

    void runReplenishment(map<int,int> itemsToReplenish) {
        if (replenishStrategy) {
            replenishStrategy->replenish(inventoryManager, itemsToReplenish);
        }
    }

    // Delegation Methods
    vector<Product*> getAllProducts() {
        return inventoryManager->getAvailableProducts();
    }

    int checkStock(int sku) {
        return inventoryManager->checkStock(sku);
    }

    void removeStock(int sku, int qty) {
        inventoryManager->removeStock(sku, qty); 
    }

    void addStock(int sku, int qty) {
        Product* prod = ProductFactory::createProduct(sku);
        inventoryManager->addStock(sku, qty);
    }

    // Getters & Setters
    void setReplenishStrategy(ReplenishStrategy* strategy) {
        this->replenishStrategy = strategy;
    }

    string getName() {
        return this->name;
    }

    double getXCoordinate() {
        return this->x;
    }

    double getYCoordinate() {
        return this->y;
    }

    InventoryManager* getInventoryManager() {
        return this->inventoryManager;
    }
};

class DarkStoreManager {
    vector<DarkStore*>* stores;
    static DarkStoreManager* instance;
    DarkStoreManager() {
        stores = new vector<DarkStore*>();
    }
    
    public:
    static DarkStoreManager* getInstance() {
        if(instance == nullptr) {
            instance = new DarkStoreManager();
        }
        return instance;
    }
    ~DarkStoreManager() {
        for (auto ds : *stores) {
            delete ds;
        }
        delete stores;
    }
    void registerDarkStore(DarkStore* darkStore){
        stores->push_back(darkStore);
    }
    vector<DarkStore*> getNearbyDarkStores(double ux, double uy, double maxDistance) {
        vector<pair<double,DarkStore*>> distList;
        for (auto ds : *stores) {
            double d = ds->distanceTo(ux, uy);
            if (d <= maxDistance) {
                distList.push_back(make_pair(d, ds));
            }
        }
        sort(distList.begin(), distList.end(),[](auto &a, auto &b){ return a.first < b.first; });

        vector<DarkStore*> result;
        for (auto &p : distList) {
            result.push_back(p.second);
        }
        return result;
    }
};
DarkStoreManager* DarkStoreManager::instance = nullptr;

class Cart {
    public:
    vector<pair<Product*,int>> items; // <Product,Quantity>
    void addItem(int sku,int qty) {
        Product* product = ProductFactory::createProduct(sku);
        items.push_back({product,qty});
        cout << "[Cart] Added SKU " << sku << " (" << product->getName() << ") x" << qty << endl;
    }
    double getTotal() {
        double amount = 0.0;
        for(auto it:items) {
            amount += ((it.first->getPrice())*it.second);
        }
        return amount;  
    }
    vector<pair<Product*,int>> getItems() {
        return items;
    }
    ~Cart() {
        for(auto it:items) {
            delete it.first;
        }
    }
};

class User {
    public:

    string name;
    double x,y;
    Cart* cart;
    
    User(string n, double x_coord , double y_coord) {
        name = n;
        x = x_coord;
        y = y_coord;
        cart = new Cart();
    }
    ~User() {
        delete cart;
    }
    Cart* getCart() {
        return cart;
    }
};

class DeliveryPartner {
public:
    string name;
    DeliveryPartner(string n) {
        name = n;
    }
};

class Order {
    public:
    static int nextId;
    int orderId;
    User* user;
    vector<pair<Product*,int>> items;
    vector<DeliveryPartner*> deliveryPartners;
    double totalAmount;
    Order(User* u) {
        orderId = nextId++;
        user = u;
        totalAmount = 0.0;
    }
};
int Order::nextId = 1;

class OrderManager {
    vector<Order*>* orders;
    static OrderManager* instance;
    OrderManager() {
        orders = new vector<Order*>();
    }
    public:
    static OrderManager* getInstance() {
        if(instance == nullptr) {
            instance = new OrderManager();
        }
        return instance;
    }

    void placeOrder(User* user, Cart* cart) {
        cout<<"\n[Order Manager] Placing Order for: "<<user->name<<"\n";
        vector<pair<Product*,int>> requestedItems = cart->getItems();
        
        //Find nearby dark stores within 5Km
        double maxDis = 5.0;
        vector<DarkStore*> nearByDarkStores = DarkStoreManager::getInstance()->getNearbyDarkStores(user->x,user->y,maxDis);
        if(nearByDarkStores.empty()) {
            cout<<"No nearby Dark Stores within 5Km.\n";
            return;
        }

        //Check if closest store has all the requested items
        DarkStore* firstStore = nearByDarkStores.front();
        bool allInFirst = true;
        for(pair<Product*,int>& item : requestedItems) {
            int sku = item.first->getSku();
            int qty = item.second;

            if(firstStore->checkStock(sku) < qty) {
                allInFirst = false;
                break;
            }
        }

        Order* order = new Order(user);

        //requirement is limited to 1 delivery partner
        if(allInFirst) {
            cout<<"All items at: "<<firstStore->getName()<<"\n";

            //Remove the products from store
            for(pair<Product*,int> item : requestedItems) {
                int sku = item.first->getSku();
                int qty = item.second;
                firstStore->removeStock(sku,qty);
                order->items.push_back({item.first,qty});
            }
            order->totalAmount = cart->getTotal();
            order->deliveryPartners.push_back(new DeliveryPartner("Partner 1"));
            cout<<"Assigned Delivery Partner : Partner 1\n";
        }
        //Multiple Delivery Partners required
        else {
            cout<<"Splitting order across stores...\n";
            
            map<int,int> allItems;  //SKU --> Qty
            for(pair<Product*,int> item :requestedItems) {
                int sku = item.first->getSku();
                int qty = item.second;
                allItems[sku] = qty;
            }
            int partnerId = 1;
            for(DarkStore* store: nearByDarkStores) {
                // If allItems becomes empty, we break early (all SKUs have been assigned)
                if(allItems.empty()) break;

                cout<<" Checking : "<<store->getName()<<"\n";
                bool assigned = false;
                vector<int> toErase;

                for(auto& [sku,qtyNeeded] : allItems) {
                    int availableQty = store->checkStock(sku);
                    if(availableQty <= 0) continue;

                    //take whichever is smaller: available or qtyNeeded.
                    int takenQty = min(availableQty,qtyNeeded);
                    store->removeStock(sku, takenQty);

                    cout<<store->getName()<<" supplies SKU "<<sku<<" x"<<takenQty<<"\n";
                    order->items.push_back({ProductFactory::createProduct(sku),takenQty});

                    // Adjust the Quantity
                    if(qtyNeeded > takenQty) {
                        allItems[sku] = qtyNeeded - takenQty;
                    }
                    else {
                        toErase.push_back(sku);
                    }
                    assigned = true;
                }

                // After iterating all SKUs in allItems, we erase 
                // any fully‐satisfied SKUs from the allItems map.
                for(int sku : toErase) allItems.erase(sku);

                // If at least one SKU was taken from this store, we assign 
                // a new DeliveryPartner.
                if(assigned) {
                    string pname = "Partner" + to_string(partnerId++);
                    order->deliveryPartners.push_back(new DeliveryPartner(pname));
                    cout<<" Assigned : "<<pname<<" for "<<store->getName()<<"\n";
                }
            }

            //  if remaining still has entries, we print which SKUs/quantities could not be fulfilled.
            if(!allItems.empty()) {
                cout<<" Could not fulfill : \n";
                for(auto& [sku,qty] : allItems) {
                    cout<<" SKU "<<sku<<" x"<<qty<<"\n";
                }
            }

            // recompute order->totalAmount
            double sum = 0;
            for(auto& item: order->items) {
                sum+=item.first->getPrice()*item.second;
            }
            order->totalAmount = sum;
        }

        // Printing Order Summary
        cout<<"\n[Order Manager] Order #"<<order->orderId<<" Summary: \n";
        cout<<" User: "<<user->name<<"\n Items:\n";
        for(auto& item: order->items) {
            cout<<" SKU"<<item.first->getSku()<<" ("<<item.first->getName()<<") x"<<item.second<<" @ Rs."<<item.first->getPrice()<<"\n";
        }
        cout<<"Total: Rs."<<order->totalAmount<<"\n Partners:\n";
        for(auto* dp:order->deliveryPartners) {
            cout<<" "<<dp->name<<"\n";
        }
        cout<<endl;
        orders->push_back(order);

        // Cleanups
        for(auto* dp : order->deliveryPartners) delete dp;
        for(auto& item : order->items) delete item.first;
    }

    vector<Order*> getAllOrders() {
        return *orders;
    }

    ~OrderManager() {
        for (auto ord : *orders) {
            delete ord;
        }
        delete orders;
    }
};
OrderManager* OrderManager::instance = nullptr;

class ZeptoHelper {
    public:
    static void showAllItems(User* user) {
        cout<<"\n[Zepto] All Available products within 5Km for "<<user->name<<":\n";

        DarkStoreManager* dsManager = DarkStoreManager::getInstance();
        vector<DarkStore*> nearByStores = dsManager->getNearbyDarkStores(user->x,user->y,5.0);

        map<int,double> skuToPrice;
        map<int,string> skuToName;

        for(DarkStore* darkStore: nearByStores) {
            vector<Product*> products = darkStore->getAllProducts();

            for(Product* product: products) {
                int sku = product->getSku();
                if(skuToPrice.count(sku) == 0) {
                    skuToPrice[sku] = product->getPrice();
                    skuToName[sku] = product->getName();
                }
            }
        }
        for(auto& entry: skuToPrice) {
            int sku = entry.first;
            double price = entry.second;
            cout<<" SKU"<<sku<<" - "<<skuToName[sku]<<" @ Rs."<<price<<"\n";
        }
    }
    static void initialize() {
        auto dsManager = DarkStoreManager::getInstance();

        DarkStore* darkStoreA = new DarkStore("DarkStoreA", 0.0 , 0.0 );
        darkStoreA->setReplenishStrategy(new ThresholdReplenishStrategy(3));

        cout<<"\nAdding stocks in DarkStoreA..."<<endl;
        darkStoreA->addStock(101,5); //Apple
        darkStoreA->addStock(102,2); //Banana

        DarkStore* darkStoreB = new DarkStore("DarkStoreB", 4.0 , 1.0 );
        darkStoreB->setReplenishStrategy(new ThresholdReplenishStrategy(3));

        cout<<"\nAdding stocks in DarkStoreB..."<<endl;
        darkStoreB->addStock(101,2); //Apple
        darkStoreB->addStock(103,7); //Chocolate

        DarkStore* darkStoreC = new DarkStore("DarkStoreC", 2.0 , 3.0 );
        darkStoreC->setReplenishStrategy(new ThresholdReplenishStrategy(3));

        cout<<"\nAdding stocks in DarkStoreC..."<<endl;
        darkStoreC->addStock(102,5); //Banana
        darkStoreC->addStock(201,7); //T-shirt

        dsManager->registerDarkStore(darkStoreA);
        dsManager->registerDarkStore(darkStoreB);
        dsManager->registerDarkStore(darkStoreC);
    }
};
int main(){

    // 1) Initialize.
    ZeptoHelper::initialize();

    // 2) A User comes on Platform
    User* user = new User("Aditya", 1.0, 1.0);
    cout <<"\nUser with name " << user->name<< " comes on platform" << endl;

    // 3) Show all available items via Zepto
    ZeptoHelper::showAllItems(user);

    // 4) User adds items to cart (some not in a single store)
    cout<<"\nAdding items to cart\n";
    Cart* cart = user->getCart();
    cart->addItem(101, 4);  // dsA has 5, dsB has 3 
    cart->addItem(102, 3);  // dsA has 2, dsC has 5
    cart->addItem(103, 2);  // dsB has 10

    // 5) Place Order
    OrderManager::getInstance()->placeOrder(user, user->cart);

    // 6) Cleanup
    delete user;
    delete DarkStoreManager::getInstance();  // deletes all DarkStores and their inventoryManagers

    return 0;
}