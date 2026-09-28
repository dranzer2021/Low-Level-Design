//Updating Concrete Observers : Logger & Notification Engine


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
    TimeStampDecorator(INotification* n): INotificationDecorator(n) {}
    string getContent() const override {
        return "[2026-09-28 08:00:00]" + notif->getContent();
    }
};
class SignatureDecorator : public INotificationDecorator {
    string signature;
    public:
    SignatureDecorator(INotification* n,const string& sign): INotificationDecorator(n) {
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
        //observers.erase(remove(observers.begin(), observers.end(), obs), observers.end());
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

class Logger : public IObserver {
    NotificationObservable* notificationObservable;
    public:

    Logger() {
        this->notificationObservable = NotificationService::getInstance()->getObservable();
        notificationObservable->addObserver(this);
    }
    Logger(NotificationObservable* observable) {
        this->notificationObservable = observable;
        notificationObservable->addObserver(this);
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
    NotificationEngine() {
        this->notificationObservable = NotificationService::getInstance()->getObservable();
        notificationObservable->addObserver(this);
    }
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

int main() {
    NotificationService* notificationService = NotificationService::getInstance();

    Logger* logger = new Logger();
    NotificationEngine* notificationEngine = new NotificationEngine();

    notificationEngine->addNotificationStrategy(new EmailStrategy("random.person@gmail.com"));
    notificationEngine->addNotificationStrategy(new SMSStrategy("+91 9876543210"));
    notificationEngine->addNotificationStrategy(new PopUpStrategy());

    INotification* notification = new SimpleNotification("Your Order is Out for Delivery! ");
    notification = new TimeStampDecorator(notification);
    notification = new SignatureDecorator(notification, "Amazon Customer Service");

    notificationService->sendNotification(notification);

    delete logger;
    delete notificationEngine;
    return 0;
}