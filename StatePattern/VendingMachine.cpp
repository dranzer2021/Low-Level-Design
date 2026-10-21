#include <iostream>
#include <string>

using namespace std;

// Forward declaration
class VendingMachine;

// Forward declarations for concrete states
class NoCoinState;
class HasCoinState;
class DispenseState;
class SoldOutState;

class VendingState {
    public:
    virtual VendingState* insertCoin(VendingMachine* machine, int coin) = 0;
    virtual VendingState* selectItem(VendingMachine* machine) = 0;
    virtual VendingState* dispense(VendingMachine* machine) = 0;
    virtual VendingState* returnCoin(VendingMachine* machine) = 0;
    virtual VendingState* refill(VendingMachine* machine, int quantity) = 0;
    virtual string getStateName() = 0;
};

class VendingMachine {
    VendingState* currentState;
    int itemCount;
    int itemPrice;
    int insertedCoins;
    
    VendingState* noCoinState;
    VendingState* hasCoinState;
    VendingState* dispenseState;
    VendingState* soldOutState;

    public:
    VendingMachine(int itemCount, int itemPrice);

    // Delegate to current state and update state based on return value
    void insertCoin(int coin);
    void selectItem();
    void dispense();
    void returnCoin();
    void refill(int quantity);

    // Print the status of Vending Machine
    void printStatus();

    // Getters for states
    VendingState* getNoCoinState() { 
        return noCoinState;
    }
    VendingState* getHasCoinState() { 
        return hasCoinState;
    }
    VendingState* getDispenseState() { 
        return dispenseState; 
    }
    VendingState* getSoldOutState() { 
        return soldOutState;
    }

     // Data access methods
    int getItemCount() { 
        return itemCount; 
    }
    void decrementItemCount() { 
        itemCount--; 
    }
    void incrementItemCount(int count = 1) {
        itemCount += count;
    }
    int getInsertedCoin() { 
        return insertedCoins;
    }
    void setInsertedCoin(int coin) { 
        insertedCoins = coin;
    }
    void addCoin(int coin) { 
        insertedCoins += coin;
    }
    int getPrice() {
        return this->itemPrice;
    }
    void setPrice(int itemPrice) {
        this->itemPrice = itemPrice;
    }

};

class NoCoinState : public VendingState {
    public:
    VendingState* insertCoin(VendingMachine* machine, int coin) override {
        machine->setInsertedCoin(coin);
        cout<<"Coin Inserted! Current Balance: Rs."<<coin<<endl;
        return machine->getHasCoinState();
    }
    VendingState* selectItem(VendingMachine* machine) override {
        cout<<"Please insert coin first!"<<endl;
        return machine->getNoCoinState();
    }
    VendingState* dispense(VendingMachine* machine) override {
        cout<<"Please insert coin and select items first!"<<endl;
        return machine->getNoCoinState();
    }
    VendingState* returnCoin(VendingMachine* machine) override {
        cout<<"No coins found!"<<endl;
        return machine->getNoCoinState();
    }
    VendingState* refill(VendingMachine* machine, int quantity) override {
        cout<<"Items refilling!"<<endl;
        machine->incrementItemCount(quantity);
        return machine->getNoCoinState();
    }
    string getStateName() override {
        return "NO_COIN";
    }
};

class HasCoinState : public VendingState {
    public:
    VendingState* insertCoin(VendingMachine* machine, int coin) override {
        machine->addCoin(coin);
        cout<<"Coins Inserted. Current Balance: Rs."<<machine->getInsertedCoin();
        return machine->getHasCoinState();
    }
    VendingState* selectItem(VendingMachine* machine) override {
        if(machine->getInsertedCoin() < machine->getPrice()) {
            cout<<"Insufficient funds! Needed Rs."<<machine->getPrice() - machine->getInsertedCoin()<<" more.\n";
            return machine->getHasCoinState();
        }

        cout<<"Item Selected. Dispensing..."<<endl;
        int change = machine->getInsertedCoin() - machine->getPrice();
        if(change) {
            cout<<"Change returned. Rs."<<change<<endl;
        }
        machine->setInsertedCoin(0);
        return machine->getDispenseState();
    }
    VendingState* dispense(VendingMachine* machine) override {
        cout<<"Please select items first!"<<endl;
        return machine->getHasCoinState();
    }
    VendingState* returnCoin(VendingMachine* machine) override {
        cout<<"Coin returned. Rs."<<machine->getInsertedCoin()<<endl;
        machine->setInsertedCoin(0);
        return machine->getHasCoinState();
    }
    VendingState* refill(VendingMachine* machine, int quantity) override {
        cout<<"Can't refill in this state! "<<endl;
        return machine->getHasCoinState();
    }
    string getStateName() override {
        return "HAS_COIN";
    }
};

class DispenseState : public VendingState {
    public:
    VendingState* insertCoin(VendingMachine* machine, int coin) override {
        cout<<"Please Wait! Item Dispensing in process...\n Coins returned Rs."<<coin<<endl;
        return machine->getDispenseState();
    }
    VendingState* selectItem(VendingMachine* machine) override {
        cout<<"Item dispensing already. Please Wait. \n";
        return machine->getDispenseState();
    }
    VendingState* dispense(VendingMachine* machine) override {
        cout<<"Item Dispensed! \n";
        machine->decrementItemCount();
        if(machine->getItemCount() > 0) {
            return machine->getNoCoinState();
        }
        else {
            cout<<"Items Sold Out! \n";
            return machine->getSoldOutState();
        }
    }
    VendingState* returnCoin(VendingMachine* machine) override {
        cout<<"Can't return coin at the moment. Try again later! \n";
        return machine->getDispenseState();
    }
    VendingState* refill(VendingMachine* machine, int quantity) override {
        cout<<"Can't refill at this state! Try again later! \n";
        return machine->getDispenseState();
    }
    string getStateName() override {
        return "DISPENSING";
    }
};

class SoldOutState : public VendingState {
    public:
     VendingState* insertCoin(VendingMachine* machine, int coin) override {
       cout << "Machine is sold out. Coin returned: Rs " << coin <<endl;
        return machine->getSoldOutState(); 
    }
    
    VendingState* selectItem(VendingMachine* machine) override {
       cout << "Machine is sold out!" <<endl;
        return machine->getSoldOutState(); 
    }
    
    VendingState* dispense(VendingMachine* machine) override {
       cout << "Machine is sold out!" <<endl;
        return machine->getSoldOutState(); 
    }
    
    VendingState* returnCoin(VendingMachine* machine) override {
       cout << "Machine is sold out. No coin inserted." <<endl;
        return machine->getSoldOutState(); 
    }

    VendingState* refill(VendingMachine* machine, int quantity) override {
        cout << "Items refilling" <<endl;
        machine->incrementItemCount(quantity);
        return machine->getNoCoinState();
    }
    
   string getStateName() override {
        return "SOLD_OUT";
    }
};


VendingMachine::VendingMachine(int itemCount, int itemPrice) {
    this->itemCount = itemCount;
    this->itemPrice = itemPrice;
    this->insertedCoins = 0;

    noCoinState = new NoCoinState();
    hasCoinState = new HasCoinState();
    dispenseState = new DispenseState();
    soldOutState = new SoldOutState();

    if(itemCount > 0) {
        currentState = noCoinState;
    }
    else {
        currentState = soldOutState;
    }
}
void VendingMachine::insertCoin(int coin){
    currentState = currentState->insertCoin(this, coin);
}
void VendingMachine::selectItem(){
    currentState = currentState->selectItem(this);
}
void VendingMachine::dispense(){
    currentState = currentState->dispense(this);
}
void VendingMachine::returnCoin(){
    currentState = currentState->returnCoin(this);
}
void VendingMachine::refill(int quantity){
    currentState = currentState->refill(this, quantity);
}
void VendingMachine::printStatus() {
    cout<<"\n---Vending Machine Status ---\n";
    cout<<"Items Remaining : "<<itemCount<<endl;
    cout<<"Inserted Coins : Rs."<<insertedCoins<<endl;
    cout<<"Current State: "<<currentState->getStateName()<<endl<<endl;
}

int main() {
    cout<<"=== Water Bottle Vending Machine ==="<<endl;

    int itemCount = 2;
    int itemPrice = 20;
    VendingMachine machine(itemCount,itemPrice);
    machine.printStatus();

     // Test scenarios - each operation potentially changes state
    cout << "1. Trying to select item without coin:" <<endl;
    machine.selectItem();  // Should ask for coin, no state change
    machine.printStatus();
    
    cout << "2. Inserting coin:" <<endl;
    machine.insertCoin(10);  // State changes to HAS_COIN
    machine.printStatus();
    
    cout << "3. Selecting item with insufficient funds:" <<endl;
    machine.selectItem();  // Insufficient funds, stays in HAS_COIN
    machine.printStatus();
    
    cout << "4. Adding more coins:" <<endl;
    machine.insertCoin(10);  // Add more money, stays in HAS_COIN
    machine.printStatus();
    
    cout << "5. Selecting item Now" <<endl;
    machine.selectItem();  // State changes to SOLD
    machine.printStatus();
    
    cout << "6. Dispensing item:" <<endl;
    machine.dispense(); // State changes to NO_COIN (items remaining)
    machine.printStatus();
    
    cout << "7. Buying last item:" <<endl;
    machine.insertCoin(20);  // State changes to HAS_COIN
    machine.selectItem();  // State changes to SOLD
    machine.dispense(); // State changes to SOLD_OUT (no items left)
    machine.printStatus();
    
    cout << "8. Trying to use sold out machine:" <<endl;
    machine.insertCoin(5);  // Coin returned, stays in SOLD_OUT

    cout << "9. Trying to use sold out machine:" <<endl;
    machine.refill(2);
    machine.printStatus(); // State changes NO_COIN

    return 0;
}