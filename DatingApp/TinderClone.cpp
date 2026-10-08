#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <memory>

using namespace std;

// Forward declarations
class User;
class UserProfile;
class Message;
class ChatRoom;
class LocationService;

class NotificationObserver {
    public:
    virtual ~NotificationObserver() {}
    virtual void update(const string& message) = 0;
};

class UserNotificationObserver : public NotificationObserver {
    string userId;
    public:
    UserNotificationObserver(const string& id) {
        userId = id;
    }
    void update(const string& message) override {
        cout<<"Notification for user "<<userId<<": "<<message<<endl;
    }
};
class NotificationService {
    map<string,NotificationObserver*> observers;
    static NotificationService* instance;
    NotificationService() {}

    public:
    static NotificationService* getInstance() {
        if(!instance) {
            instance = new NotificationService();
        }
        return instance;
    }
    void registerObserver(const string& userId, NotificationObserver* observer) {
        observers[userId] = observer;
    }
    void removeObserver(string userId) {
        observers.erase(userId);
    }
    void notifyUser(string userId, string message) {
        if(observers.find(userId) != observers.end()) {
            observers[userId]->update(message);
        }
    }
    void notifyAll(const string& message) {
        for(auto& pair: observers) {
            pair.second->update(message);
        }
    }
};
NotificationService* NotificationService::instance = nullptr;

enum class SwipeAction {
    LEFT,
    RIGHT
};

class User {
    string id;
    UserProfile* profile;
    Preference* preference;
    map<string,SwipeAction> swipeHistory;
    NotificationObserver* notificationObserver;

    public:
    User(const string& userId) {
        id = userId;
        profile = new UserProfile();
        preference = new Preference();
        notificationObserver = new UserNotificationObserver(userId);
        NotificationService::getInstance()->registerObserver(userId, notificationObserver);
    }
    string getId() const {
        return id;
    }
    
    UserProfile* getProfile() {
        return profile;
    }
    
    Preference* getPreference() {
        return preference;
    }
    
    void swipe(const string& otherUserId, SwipeAction action) {
        swipeHistory[otherUserId] = action;
    }
    
    bool hasLiked(const string& otherUserId) const {
        auto it = swipeHistory.find(otherUserId);
        if (it != swipeHistory.end()) {
            return it->second == SwipeAction::RIGHT;
        }
        return false;
    }
    
    bool hasDisliked(const string& otherUserId) const {
        auto it = swipeHistory.find(otherUserId);
        if (it != swipeHistory.end()) {
            return it->second == SwipeAction::LEFT;
        }
        return false;
    }
    
    bool hasInteractedWith(const string& otherUserId) const {
        return swipeHistory.find(otherUserId) != swipeHistory.end();
    }
    
    void displayProfile() const {  // Principle of least knowledge
        profile->display();
    }
    ~User() {
        delete profile;
        delete preference;
        delete notificationObserver;
    }
};

enum class Gender {
    MALE,
    FEMALE,
    NON_BINARY,
    OTHER
};
class Interest {
    string name;
    string category;
    public:
    Interest (const string& n,const string& c){
        name = n;
        category = c;
    }
    string getName() const {
        return name;
    }
    string getCategory() const {
        return category;
    }
};
class Location {
    double lat;
    double lon;
    public:
    Location() {
        lat = 0.0;
        lon = 0.0;
    }
    Location (double latitude,double longitude) {
        lat = latitude;
        lon = longitude;
    }
    double getLatitude() const {
        return lat;
    }
    
    double getLongitude() const {
        return lon;
    }
    
    void setLatitude(double latitude) {
        lat = latitude;
    }
    
    void setLongitude(double longitude) {
        lon = longitude;
    }
    double distanceInKm(const Location& other) const {
        const double earthRadiusKm = 6371.0;
        double dLat = (other.lat - lat) * M_PI / 180.0;
        double dLon = (other.lon - lon) * M_PI / 180.0;
        
        double a = sin(dLat/2) * sin(dLat/2) +
                   cos(lat * M_PI / 180.0) * cos(other.lat * M_PI / 180.0) *
                   sin(dLon/2) * sin(dLon/2);
        double c = 2 * atan2(sqrt(a), sqrt(1-a));
        return earthRadiusKm * c;

    }
};
class UserProfile {
    string name;
    int age;
    Gender gender;
    string bio;
    Location location;
    vector<Interest*> interests;
    vector<string> photos;
    
    public:
     UserProfile() {
        name = "";
        age = 0;
        gender = Gender::OTHER;
    }
    
    ~UserProfile() {
        for (auto interest : interests) {
            delete interest;
        }
    }
    
    void setName(const string& n) {
        name = n;
    }
    
    void setAge(int a) {
        age = a;
    }
    
    void setGender(Gender g) {
        gender = g;
    }
    
    void setBio(const string& b) {
        bio = b;
    }
    
    void addPhoto(const string& photoUrl) {
        photos.push_back(photoUrl);
    }
    
    void removePhoto(const string& photoUrl) {
        photos.erase(remove(photos.begin(), photos.end(), photoUrl), photos.end());
    }
    void addInterest(const string& name, const string& category) {
        Interest* interest = new Interest(name, category);
        interests.push_back(interest);
    }
    
    void removeInterest(const string& name) {
        auto it = find_if(interests.begin(), interests.end(), 
            [&name](const Interest* interest) {
                return interest->getName() == name;
            });
        
        if (it != interests.end()) {
            delete *it;
            interests.erase(it);
        }
    }
    
    void setLocation(const Location& loc) {
        location = loc;
    }
    
    string getName() const {
        return name;
    }
    
    int getAge() const {
        return age;
    }
    
    Gender getGender() const {
        return gender;
    }
    
    string getBio() const {
        return bio;
    }
    
    const vector<string>& getPhotos() const {
        return photos;
    }
    
    const vector<Interest*>& getInterests() const {
        return interests;
    }
    
    const Location& getLocation() const {
        return location;
    }
    
    void display() const {
        cout << "===== Profile =====" << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Gender: ";
        switch (gender) {
            case Gender::MALE: cout << "Male"; break;
            case Gender::FEMALE: cout << "Female"; break;
            case Gender::NON_BINARY: cout << "Non-binary"; break;
            case Gender::OTHER: cout << "Other"; break;
        }
        cout << endl;
        
        cout << "Bio: " << bio << endl;
        
        cout << "Photos: ";
        for (const auto& photo : photos) {
            cout << photo << ", ";
        }
        cout << endl;
        
        cout << "Interests: ";
        for (const auto& interest : interests) {
            cout << interest->getName() << " (" << interest->getCategory() << "), ";
        }
        cout << endl;
        
        cout << "Location: " << location.getLatitude() << ", " << location.getLongitude() << endl;
        cout << "===================" << endl;
    }
};

class Preference {
    int minAge;
    int maxAge;
    double maxDistance;
    vector<string> interests;
    vector<Gender> interestedIn;

    public:
    Preference() {
        minAge = 18;
        maxAge = 100;
        maxDistance = 100.0;
    }
    void addGenderPreference(Gender gender) {
        interestedIn.push_back(gender);
    }
    void removeGenderPreference(Gender gender) {
        interestedIn.erase(remove(interestedIn.begin(),interestedIn.end(),gender),interestedIn.end());
    }
    void setAgeRange(int min,int max) {
        minAge = min;
        maxAge = max;
    }
    void setMaxDistance(double distance) {
        maxDistance = distance;
    }
    
    void addInterest(const string& interest) {
        interests.push_back(interest);
    }
    
    void removeInterest(const string& interest) {
        interests.erase(remove(interests.begin(), interests.end(), interest), interests.end());
    }
    
    bool isInterestedInGender(Gender gender) const {
        return find(interestedIn.begin(), interestedIn.end(), gender) != interestedIn.end();
    }
    
    bool isAgeInRange(int age) const {
        return age >= minAge && age <= maxAge;
    }
    
    bool isDistanceAcceptable(double distance) const {
        return distance <= maxDistance;
    }
    
    const vector<string>& getInterests() const {
        return interests;
    }
    
    const vector<Gender>& getInterestedGenders() const {
        return interestedIn;
    }
    
    int getMinAge() const {
        return minAge;
    }
    
    int getMaxAge() const {
        return maxAge;
    }
    
    double getMaxDistance() const {
        return maxDistance;
    }

};

class Message {
    string senderId;
    string content;
    time_t timestamp;
    
    public:
    Message(const string& sender, const string& msg) {
        senderId = sender;
        content = msg;
        timestamp = time(nullptr);
    }
    
    string getSenderId() const {
        return senderId;
    }
    
    string getContent() const {
        return content;
    }
    
    time_t getTimestamp() const {
        return timestamp;
    }
    
    string getFormattedTime() const {
        struct tm* timeinfo;
        char buffer[80];
        
        timeinfo = localtime(&timestamp);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return string(buffer);
    }
};
class ChatRoom {
private:
    string id;
    vector<string> participantIds;
    vector<Message*> messages;
    
public:
    ChatRoom(const string& roomId, const string& user1Id, const string& user2Id) {
        id = roomId;
        participantIds.push_back(user1Id);
        participantIds.push_back(user2Id);
    }
    
    ~ChatRoom() {
        for (auto msg : messages) {
            delete msg;
        }
    }
    
    string getId() const {
        return id;
    }
    
    void addMessage(const string& senderId, const string& content) {
        Message* msg = new Message(senderId, content);
        messages.push_back(msg);
    }
    
    bool hasParticipant(const string& userId) const {
        return find(participantIds.begin(), participantIds.end(), userId) != participantIds.end();
    }
    
    const vector<Message*>& getMessages() const {
        return messages;
    }
    
    const vector<string>& getParticipants() const {
        return participantIds;
    }
    
    void displayChat() const {
        cout << "===== Chat Room: " << id << " =====" << endl;
        for (const auto& msg : messages) {
            cout << "[" << msg->getFormattedTime() << "] " 
                 << msg->getSenderId() << ": " << msg->getContent() << endl;
        }
        cout << "=========================" << endl;
    }
};

class LocationStrategy {
    public:
    virtual ~LocationStrategy() {}
    virtual vector<User*> findNearbyUsers(const Location& loc,double maxDis,const vector<User*>& allUsers) = 0;
};
class BasicLocationStrategy : public LocationStrategy {
    public:
    vector<User*> findNearbyUsers(const Location& location,double maxDistance,const vector<User*>& allUsers) override {
        vector<User*> nearbyUsers;
        for (User* user : allUsers) {
            double distance = location.distanceInKm(user->getProfile()->getLocation());
            if (distance <= maxDistance) {
                nearbyUsers.push_back(user);
            }
        }
        return nearbyUsers;
    }
};

class LocationService {
    static LocationService* instance;
    LocationStrategy* strategy;

    LocationService () {
        strategy = new BasicLocationStrategy();
    }
    public:
    static LocationService* getInstance() {
        if(!instance) {
            instance = new LocationService();
        }
        return instance;
    }
    ~LocationService() {
        delete strategy;
    }
    
    void setStrategy(LocationStrategy* newStrategy) {
        delete strategy;
        strategy = newStrategy;
    }
    
    vector<User*> findNearbyUsers(const Location& location, double maxDistance, const vector<User*>& allUsers) {
        return strategy->findNearbyUsers(location, maxDistance, allUsers);
    }
};
LocationService* LocationService::instance = nullptr;


int main() {

    return 0;
}