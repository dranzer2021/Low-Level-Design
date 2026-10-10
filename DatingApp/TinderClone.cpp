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
class Preference;

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

enum class MatcherType {
    BASIC,
    INTERESTS_BASED,
    LOCATION_BASED
};

//Chain of Responsibitily Design Pattern
class Matcher {
    public:
    virtual ~Matcher() {}
    virtual double calculateMatchScore(User* user1, User* user2) = 0;
};

class BasicMatcher : public Matcher {
    public:
    double calculateMatchScore(User* user1, User* user2) override {
        //Gender Preference Matching
        bool user1LikesUser2Gender = user1->getPreference()->isInterestedInGender(user2->getProfile()->getGender());
        bool user2LikesUser1Gender = user2->getPreference()->isInterestedInGender(user1->getProfile()->getGender());
        if(!user1LikesUser2Gender || !user2LikesUser1Gender)
            return 0.0;
        
        //Age Preference Matching
        bool user1LikesUser2Age = user1->getPreference()->isAgeInRange(user2->getProfile()->getAge());
        bool user2LikesUser1Age = user2->getPreference()->isAgeInRange(user1->getProfile()->getAge());
        if(!user1LikesUser2Age || !user2LikesUser1Age)
            return 0.0;

        //Distance Preference Matching
        double distance = user1->getProfile()->getLocation().distanceInKm(user2->getProfile()->getLocation());
        bool user1LikesUser2Distance = user1->getPreference()->isDistanceAcceptable(distance);
        bool user2LikesUser1Distance = user2->getPreference()->isDistanceAcceptable(distance);
        if(!user1LikesUser2Distance || !user2LikesUser1Distance) 
            return 0.0;

        //If all three matched 50% score alloted
        return 0.5;
    }
};

class InterestsBasedMatcher : public Matcher {
    public:
    double calculateMatchScore(User* user1, User* user2) override {
        BasicMatcher basicMatcher;
        double baseScore = basicMatcher.calculateMatchScore(user1,user2);

        if(baseScore == 0.0) 
            return 0.0;

        vector<string> user1InterestNames;
        for(const auto& interest: user1->getProfile()->getInterests()){
            user1InterestNames.push_back(interest->getName());
        }

        int sharedInterests = 0;
        for(const auto& interest: user2->getProfile()->getInterests()){
            if(find(user1InterestNames.begin(),user1InterestNames.end(),interest->getName()) != user1InterestNames.end())
                sharedInterests++;
        }

        // Bonus score based on shared interests (up to 0.5 additional points)
        double maxInterests = max(user1->getProfile()->getInterests().size() , user2->getProfile()->getInterests().size());
        double interestScore = maxInterests > 0 ? 0.5*(sharedInterests/maxInterests) : 0.0;

        return baseScore+interestScore;
        
    }
};

class LocationBasedMatcher : public Matcher {
    public:
    double calculateMatchScore(User* user1, User* user2) override {
        InterestsBasedMatcher interestsBasedMatcher;
        double baseScore = interestsBasedMatcher.calculateMatchScore(user1,user2);

        if(baseScore == 0.0) 
            return 0.0;
        
        double distance = user1->getProfile()->getLocation().distanceInKm(user2->getProfile()->getLocation());
        double maxDistance = min(user1->getPreference()->getMaxDistance(),user2->getPreference()->getMaxDistance());

        // Closer is better, score decreases with distance (up to 0.2 additional points)
        double proximityScore = maxDistance > 0 ? 0.2*(1.0 - (distance / maxDistance)) : 0.0;
        return baseScore + proximityScore;
    }
};

class MatcherFactory {
    public:
    static Matcher* createMatcher(MatcherType type) {
        switch(type) {
            case MatcherType::BASIC:
                return new BasicMatcher();
            case MatcherType::INTERESTS_BASED:
                return new InterestsBasedMatcher();
            case MatcherType::LOCATION_BASED:
                return new LocationBasedMatcher();
            default:
                return new BasicMatcher();
        }
    }
};

class DatingApp {
    vector<User*> users;
    vector<ChatRoom*> chatRooms;
    Matcher* matcher;
    static DatingApp* instance;

    DatingApp() {
        matcher = MatcherFactory::createMatcher(MatcherType::LOCATION_BASED);
    }

    public:

    static DatingApp* getInstance() {
        if(!instance) {
            instance = new DatingApp();
        }
        return instance;
    }

    ~DatingApp() {
        for(auto user: users) {
            delete user;
        }
        for(auto chatRoom: chatRooms) {
            delete chatRoom;
        }
        delete matcher;
    }

    void setMatcher(MatcherType type) {
        delete matcher;
        matcher = MatcherFactory::createMatcher(type);
    }
    User* createUser(const string& userId) {
        User* user = new User(userId);
        users.push_back(user);
        return user;
    }
    User* getUserById(const string& userId) {
        for(auto user: users) {
            if(user->getId() == userId)
                return user;
        }
        return nullptr;
    }
    vector<User*> findNearbyUsers(const string& userId, double maxDistance = 5.0) {
        User* user = getUserById(userId);
        if(user == nullptr) {
            return vector<User*>();
        }

        // Find users within maxDistance km
        vector<User*> nearbyUsers = LocationService::getInstance()->findNearbyUsers(
            user->getProfile()->getLocation(),maxDistance,users);

        //Remove the user itself
        nearbyUsers.erase(remove(nearbyUsers.begin(),nearbyUsers.end(),user),nearbyUsers.end());
        
        //Filter out users that don't match preferences or have already been swiped
        vector<User*> filteredUsers;
        for(User* otherUser: nearbyUsers) {

            // Skip users that have already been interacted with
            if(user->hasInteractedWith(otherUser->getId()))
                continue;

            // Calculate match score
            double score = matcher->calculateMatchScore(user,otherUser);

            // If score is above 0, they meet basic preference criteria
            if(score > 0) {
                filteredUsers.push_back(otherUser);
            }
            
        }
        return filteredUsers;
    }

    bool swipe(const string& userId, const string& targetUserId , SwipeAction action) {
        User* user = getUserById(userId);
        User* targetUser = getUserById(targetUserId);

        if(user == nullptr || targetUser == nullptr) {
            cout<<"User not found."<<endl;
            return false;
        }
        user->swipe(targetUserId,action);

        if(action == SwipeAction::RIGHT && targetUser->hasLiked(userId)) {
            //It's a match
            string chatRoomId = userId + "_" + targetUserId;
            ChatRoom* chatRoom = new ChatRoom(chatRoomId, userId, targetUserId);
            chatRooms.push_back(chatRoom);

            //Notify both
            
            NotificationService::getInstance()->notifyUser(userId, "You have a new match with " + targetUser->getProfile()->getName() + " !");
            NotificationService::getInstance()->notifyUser(targetUserId, "You have a new match with " + user->getProfile()->getName() + " !");
        
            return true;
        }

        return false;
    }

    ChatRoom* getChatRoom(const string& user1Id, const string& user2Id) {
        for (auto chatRoom : chatRooms) {
            if (chatRoom->hasParticipant(user1Id) && chatRoom->hasParticipant(user2Id)) {
                return chatRoom;
            }
        }
        return nullptr;
    }

    void sendMessage(const string& senderId, const string& receiverId, const string& content) {
        ChatRoom* chatRoom = getChatRoom(senderId, receiverId);
        if (chatRoom == nullptr) {
            cout << "No chat room found between these users." << endl;
            return;
        }
        
        chatRoom->addMessage(senderId, content);
        
        // Notify the receiver
        NotificationService::getInstance()->notifyUser(receiverId, "New message from " + getUserById(senderId)->getProfile()->getName());
    }

    void displayUser(const string& userId) {
        User* user = getUserById(userId);
        if (user == nullptr) {
            cout << "User not found." << endl;
            return;
        }
        
        user->displayProfile();
    }
    
    void displayChatRoom(const string& user1Id, const string& user2Id) {
        ChatRoom* chatRoom = getChatRoom(user1Id, user2Id);
        if (chatRoom == nullptr) {
            cout << "No chat room found between these users." << endl;
            return;
        }
        
        chatRoom->displayChat();
    }
};
DatingApp* DatingApp::instance = nullptr;

int main() {

    DatingApp* app = DatingApp::getInstance();
    
    //Create Users
    User* user1 = app->createUser("user1");
    User* user2 = app->createUser("user2");
    
     // Set user1 profile
    UserProfile* profile1 = user1->getProfile();
    profile1->setName("Rohan");
    profile1->setAge(28);
    profile1->setGender(Gender::MALE);
    profile1->setBio("I am a software developer");
    profile1->addPhoto("rohan_photo1.jpg");
    profile1->addInterest("Coding", "Programming");
    profile1->addInterest("Travel", "Lifestyle");
    profile1->addInterest("Music", "Entertainment");
    
    // Setup user1 preferences
    Preference* pref1 = user1->getPreference();
    pref1->addGenderPreference(Gender::FEMALE);
    pref1->setAgeRange(25, 30);
    pref1->setMaxDistance(10.0);
    pref1->addInterest("Coding");
    pref1->addInterest("Travel");

    // Setup user2 profile
    UserProfile* profile2 = user2->getProfile();
    profile2->setName("Neha");
    profile2->setAge(27);
    profile2->setGender(Gender::FEMALE);
    profile2->setBio("Art teacher who loves painting and traveling.");
    profile2->addPhoto("neha_photo1.jpg");
    profile2->addInterest("Painting", "Art");
    profile2->addInterest("Travel", "Lifestyle");
    profile2->addInterest("Music", "Entertainment");
    
    // Setup user2 preferences
    Preference* pref2 = user2->getPreference();
    pref2->addGenderPreference(Gender::MALE);
    pref2->setAgeRange(27, 30);
    pref2->setMaxDistance(15.0);
    pref2->addInterest("Coding");
    pref2->addInterest("Movies");

    // Set location for user1
    Location location1;
    location1.setLatitude(1.01);
    location1.setLongitude(1.02);
    profile1->setLocation(location1);
    
    // Set location for user2 (Close to user1, within 5km)
    Location location2;
    location2.setLatitude(1.03);
    location2.setLongitude(1.04);
    profile2->setLocation(location2);

    // Display user profiles
    std::cout << "---- User Profiles ----" << std::endl;
    app->displayUser("user1");
    app->displayUser("user2");
    
    // Find nearby users for user1 (within 5km)
    std::cout << "\n---- Nearby Users for user1 (within 5km) ----" << std::endl;
    std::vector<User*> nearbyUsers = app->findNearbyUsers("user1", 5.0);
    std::cout << "Found " << nearbyUsers.size() << " nearby users" << std::endl;
    for (User* user : nearbyUsers) {
        std::cout << "- " << user->getProfile()->getName() << " (" << user->getId() << ")" << std::endl;
    }
    
    // User1 swipes right on User2
    std::cout << "\n---- Swipe Actions ----" << std::endl;
    std::cout << "User1 swipes right on User2" << std::endl;
    app->swipe("user1", "user2", SwipeAction::RIGHT);
    
    // User2 swipes right on User1 (creating a match)
    std::cout << "User2 swipes right on User1" << std::endl;
    app->swipe("user2", "user1", SwipeAction::RIGHT);
    
    // Send messages in the chat room
    std::cout << "\n---- Chat Room ----" << std::endl;
    app->sendMessage("user1", "user2", "Hi Neha  ");
    
    app->sendMessage("user2", "user1", "Hi Rohan ");
    
    // Display the chat room
    app->displayChatRoom("user1", "user2");

    return 0;
}