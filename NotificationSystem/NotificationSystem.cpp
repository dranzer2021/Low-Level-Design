#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

 //Notification & Decorators
class INotification {
    public:
    virtual string getContent() const = 0;
    virtual ~INotification() {}
};
class SimpleNotification: public INotification {
    string text;
    public:
    SimpleNotification(const string& content) {
        text = content;
    }
    string getContent() const override {
        return text;
    }
};
class INotificationDecorator : public INotification {
    protected:
    INotification* notif;
    public:
    INotificationDecorator(INotification* n) {
        notif = n;
    }
    virtual ~INotificationDecorator() {
        delete notif;
    }
};
class TimeStampDecorator : public INotificationDecorator {
    public:
    TimeStampDecorator(INotification* n): INotificationDecorator(n) {
        //notif = n;
    }
    string getContent() const override {
        return "[2026-09-28 08:00:00]" + notif->getContent();
    }
};
class SignatureDecorator : public INotificationDecorator {
    string signature;
    public:
    SignatureDecorator(INotification* n,const string& sign): INotificationDecorator(n) {
        //notif = n;
        signature = sign;
    }
    string getContent() const override {
        return notif->getContent() + "\n --" + signature + "\n\n";
    }
};


//Observer Pattern Components

class IObserver {
    public:
    virtual void update() = 0;
    virtual ~IObserver() {}
};
class IObservable {
    public:
    virtual void addObserver(IObserver* o) = 0;
    virtual void removeObserver(IObserver* o) = 0;
    virtual void notifyObservers() = 0;
};
class NotificationObservable : public IObservable {
    vector<IObserver*> observers;
    INotification* currentNotification;
    public:
     NotificationObservable() { 
        currentNotification = nullptr; 
    }
    void addObserver(IObserver* o) override {
        observers.push_back(o);
    }
    void removeObserver(IObserver* o) override {
        for(auto it:observers) {
            if(o==it){
                swap(it,observers.back());
                observers.pop_back();
                break;
            }
        }
    }
    void notifyObservers() override {
        for (unsigned int i = 0; i < observers.size(); i++) {
            observers[i]->update();
        }
    }
    void setNotification(INotification* n) {
        if (currentNotification != nullptr) {
            delete currentNotification;
        }
        currentNotification = n;
        notifyObservers();
    }
    INotification* getNotification() {
        return currentNotification;
    }
    string getNotificationContent() {
        return currentNotification->getContent();
    }

    ~NotificationObservable() {
        if (currentNotification != NULL) {
            delete currentNotification;
        }
    }
};
class Logger : public IObserver {
    NotificationObservable* notificationObservable;
    public:
    Logger(NotificationObservable* observable) {
        this->notificationObservable = observable;
    }
    void update() {
        cout << "Logging New Notification : \n" << notificationObservable->getNotificationContent();
    }
};

class INotificationStrategy {
    public:
    virtual void sendNotification(string content) = 0;
};
class EmailStrategy : public INotificationStrategy {
    string emailId;
    public:
    EmailStrategy(string emailId) {
        this->emailId = emailId;
    }
    void sendNotification(string content) override {
        cout<<"Sending notification to emailID : "<<emailId<<"\n"<< content;
    }
};
class SMSStrategy : public INotificationStrategy {
    string mobileNumber;
    public:
    SMSStrategy(string mobileNumber) {
        this->mobileNumber = mobileNumber;
    }
    void sendNotification(string content) override {
        cout<<"Sending notification to mobile number : "<<mobileNumber<<"\n"<< content;
    }
};
class PopUpStrategy : public INotificationStrategy {
    
    public:
    void sendNotification(string content) override {
        cout<<"Sending pop-up notification : \n"<< content;
    }
};

class NotificationEngine : public IObserver {
    NotificationObservable* notificationObservable;
    vector<INotificationStrategy*> notificationStrategies;
    public:
    NotificationEngine(NotificationObservable* observable) {
        this->notificationObservable = observable;
    }

    void addNotificationStrategy(INotificationStrategy* ns) {
        this->notificationStrategies.push_back(ns);
    }
    void update() {
        string notificationContent = notificationObservable->getNotificationContent();
        for(const auto notificationStrategy : notificationStrategies) {
            notificationStrategy->sendNotification(notificationContent);
        }
    }
};

class NotificationService {
    NotificationObservable* observable;
    vector<INotification*> notifications;
    static NotificationService* instance;
    NotificationService() {
        // private constructor
        observable = new NotificationObservable();
    }
    public:
    static NotificationService* getInstance() {
        if(!instance) {
            instance = new NotificationService();
        }
        return instance;
    }
    NotificationObservable* getObservable() {
        return observable;
    }
    void sendNotification(INotification* notification) {
        notifications.push_back(notification);
        observable->setNotification(notification);
    }
    ~NotificationService() {
        delete observable;
    }
};
NotificationService* NotificationService::instance = nullptr;

int main() {
    NotificationService* notificationService = NotificationService::getInstance();
    NotificationObservable* notificationObservable = notificationService->getObservable();

    Logger* logger = new Logger(notificationObservable);
    NotificationEngine* notificationEngine = new NotificationEngine(notificationObservable);

    notificationEngine->addNotificationStrategy(new EmailStrategy("random.person@gmail.com"));
    notificationEngine->addNotificationStrategy(new SMSStrategy("+91 9876543210"));
    notificationEngine->addNotificationStrategy(new PopUpStrategy());

    notificationObservable->addObserver(logger);
    notificationObservable->addObserver(notificationEngine);

    INotification* notification = new SimpleNotification("Your Order is Out for Delivery! ");
    notification = new TimeStampDecorator(notification);
    notification = new SignatureDecorator(notification, "Amazon Customer Service");

    notificationService->sendNotification(notification);

    delete logger;
    delete notificationEngine;
    return 0;
}


/*
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//Notification & Decorators

class INotification {
public:
    virtual string getContent() const = 0;

    virtual ~INotification() {}
};

// Concrete Notification: simple text notification.
class SimpleNotification : public INotification {
private:
    string text;
public:
    SimpleNotification(const string& msg) {
        text = msg;
    }
    string getContent() const override {
        return text;
    }
};

// Abstract Decorator: wraps a Notification object.
class INotificationDecorator : public INotification {
protected:
    INotification* notification;
public:
    INotificationDecorator(INotification* n) {
        notification = n;
    }
    virtual ~INotificationDecorator() {
        delete notification;
    }
};

// Decorator to add a timestamp to the content.
class TimestampDecorator : public INotificationDecorator {
public:
    TimestampDecorator(INotification* n) : INotificationDecorator(n) { }
    
    string getContent() const override {
        return "[2025-04-13 14:22:00] " + notification->getContent();
    }
};

// Decorator to append a signature to the content.
class SignatureDecorator : public INotificationDecorator {
private:
    string signature;
public:
    SignatureDecorator(INotification* n, const string& sig) : INotificationDecorator(n) {
        signature = sig;
    }
    string getContent() const override {
        return notification->getContent() + "\n-- " + signature + "\n\n";
    }
};

//Observer Pattern Components

// Observer interface: each observer gets an update with a Notification pointer.
class IObserver {
public:
    virtual void update() = 0;

    virtual ~IObserver() {}
};

class IObservable {
public:
    virtual void addObserver(IObserver* observer) = 0;
    virtual void removeObserver(IObserver* observer) = 0;
    virtual void notifyObservers() = 0;
};

// Concrete Observable
class NotificationObservable :  public IObservable {
private:
    vector<IObserver*> observers;
    INotification* currentNotification;
public:
    NotificationObservable() { 
        currentNotification = nullptr; 
    }

    void addObserver(IObserver* obs) override {
        observers.push_back(obs);
    }

    void removeObserver(IObserver* obs) override {
        observers.erase(remove(observers.begin(), observers.end(), obs), observers.end());
    }

    void notifyObservers() override {
        for (unsigned int i = 0; i < observers.size(); i++) {
            observers[i]->update();
        }
    }

    void setNotification(INotification* notification) {
        if (currentNotification != nullptr) {
            delete currentNotification;
        }
        currentNotification = notification;
        notifyObservers();
    }

    INotification* getNotification() {
        return currentNotification;
    }

    string getNotificationContent() {
        return currentNotification->getContent();
    }

    ~NotificationObservable() {
        if (currentNotification != NULL) {
            delete currentNotification;
        }
    }
};
    
// Concrete Observer 1
class Logger : public IObserver {
private:
    NotificationObservable* notificationObservable;

public:
    Logger(NotificationObservable* observable) {
        this->notificationObservable = observable;
    }

    void update() {
        cout << "Logging New Notification : \n" << notificationObservable->getNotificationContent();
    }
};

// Strategy Pattern Components 
class INotificationStrategy {
public:    
    virtual void sendNotification(string content) = 0;
};

class EmailStrategy : public INotificationStrategy {
private:
    string emailId;
public:

    EmailStrategy(string emailId) {
        this->emailId = emailId;
    }

    void sendNotification(string content) override {
        // Simulate the process of sending an email notification, 
        // representing the dispatch of messages to users via email.​
        cout << "Sending email Notification to: " << emailId << "\n" << content;
    }
};

class SMSStrategy : public INotificationStrategy {
private:
    string mobileNumber;
public:

    SMSStrategy(string mobileNumber) {
        this->mobileNumber = mobileNumber;
    }

    void sendNotification(string content) override {
        // Simulate the process of sending an SMS notification, 
        // representing the dispatch of messages to users via SMS.​
        cout << "Sending SMS Notification to: " << mobileNumber << "\n" << content;
    }
};

class PopUpStrategy : public INotificationStrategy {
public:
    void sendNotification(string content) override {
        // Simulate the process of sending popup notification.
        cout << "Sending Popup Notification: \n" << content;
    }
};

class NotificationEngine : public IObserver {
private:
    NotificationObservable* notificationObservable;
    vector<INotificationStrategy*> notificationStrategies;

public:
    NotificationEngine(NotificationObservable* observable) {
        this->notificationObservable = observable;
    }

    void addNotificationStrategy(INotificationStrategy* ns) {
        this->notificationStrategies.push_back(ns);
    }

    // Can have RemoveNotificationStrategy as well.

    void update() {
        string notificationContent = notificationObservable->getNotificationContent();
        for(const auto notificationStrategy : notificationStrategies) {
            notificationStrategy->sendNotification(notificationContent);
        }
    }
};

//NotificationService

// The NotificationService manages notifications. It keeps track of notifications. 
// Any client code will interact with this service.

// Singleton class
class NotificationService {
private:
    NotificationObservable* observable;
    static NotificationService* instance;
    vector<INotification*> notifications;

    NotificationService() {
        // private constructor
        observable = new NotificationObservable();
    }

public:
    static NotificationService* getInstance() {
        if(instance == nullptr) {
            instance = new NotificationService();
        }
        return instance;
    }

    // Expose the observable so observers can attach.
    NotificationObservable* getObservable() {
        return observable;
    }

    // Creates a new Notification and notifies observers.
    void sendNotification(INotification* notification) {
        notifications.push_back(notification); // history
        observable->setNotification(notification);
    }

    ~NotificationService() {
        delete observable;
    }
};

NotificationService* NotificationService::instance = nullptr;

int main() {
    // Create NotificationService.
    NotificationService* notificationService = NotificationService::getInstance();

    // Get Observable
    NotificationObservable* notificationObservable = notificationService->getObservable();
   
    // Create Logger Observer
    Logger* logger = new Logger(notificationObservable);

    // Create NotificationEngine observers.
    NotificationEngine* notificationEngine = new NotificationEngine(notificationObservable);

    notificationEngine->addNotificationStrategy(new EmailStrategy("random.person@gmail.com"));
    notificationEngine->addNotificationStrategy(new SMSStrategy("+91 9876543210"));
    notificationEngine->addNotificationStrategy(new PopUpStrategy());

    // Attach these observers.
    notificationObservable->addObserver(logger);
    notificationObservable->addObserver(notificationEngine);

    // Create a notification with decorators.
    INotification* notification = new SimpleNotification("Your order has been shipped!");
    notification = new TimestampDecorator(notification);
    notification = new SignatureDecorator(notification, "Customer Care");
    
    notificationService->sendNotification(notification);

    delete logger;
    delete notificationEngine;
    return 0;
}
*/