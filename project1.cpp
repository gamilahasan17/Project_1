#define _CRT_SECURE_NO_WARNINGS  // Deprecation Warnings
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include<fstream>
using namespace std;

enum ChatType {
  PRIVATE_CHAT,
  GROUP_CHAT
};

// ========================
//       USER CLASS
// ========================
class User {
private:
    string username;
    string password;
    string phoneNumber;
    string status;
    string lastSeen;

public:
    User() {
        username = " ";
        password = " ";
        phoneNumber = " ";
        status = "Offline";
        time_t now = time(0);
        lastSeen = ctime(&now);
    }

    User(string uname, string pwd, string phone) {
        username = uname;
        password = pwd;
        phoneNumber = phone;
        status = "Offline";
        time_t now = time(0);
        lastSeen = ctime(&now);
    }

    string getUsername() const {
        return username;
    }

    string getPhoneNumber() const {
        return phoneNumber;
    }

    string getStatus() const {
        return status;
    }

    string getLastSeen() const {
        return lastSeen;
    }

    void setStatus(string newStatus) {
        status = newStatus;
    }

    void setPhoneNumber(string phone) {
        phoneNumber = phone;
    }

    void updateLastSeen() {
        time_t now = time(0);
        lastSeen = ctime(&now);
    }

    bool checkPassword(string pwd) const {
        return password == pwd;
    }

    void changePassword(string newPwd) {
        password = newPwd;
    }
};

// ========================
//      MESSAGE CLASS
// ========================
class Message {
private:
    string sender;
    string content;
    string timestamp;
    string status;
    Message* replyTo;

public:
    Message() {
        sender = " ";
        content = " ";
        timestamp = " ";
        status = " ";
        replyTo = nullptr;

    }

    Message(string sndr, string cntnt) {
        sender = sndr;
        content = cntnt;
        timestamp = getTimestamp();
        status = "sent";
        replyTo = nullptr;


    }

    string getContent() const {
        return content;

    }

    string getSender() const {
        return sender;
    }

    string getTimestamp() const {
        return timestamp;

    }

    string getStatus() const {
        return status;
    }

    Message* getReplyTo() const {

        return replyTo;
    }

    void setStatus(string newStatus) {
        status = newStatus;
    }

    void setReplyTo(Message* msg) {
        replyTo = msg;
    }

    void updateTimestamp() {

        time_t now = time(nullptr);       //the current time :seconds
        char arr[100];
        strftime(arr, sizeof(arr), " %Y - %m - %d %H: %M : %S", localtime(&now));


        timestamp = string(arr);

    }

    void display() const {

        cout << "Sender: " << sender << endl;
        cout << "Content: " << content << endl;
        cout << "Timestamp: " << timestamp << endl;
        cout << "Status: " << status << endl;

        if (replyTo != nullptr) {
            cout << "Replying to: " << (replyTo->getContent());

        }
    }

    void addEmoji(string emojiCode) {

        if (emojiCode == ":)")
            content += " 🙂";
        else if
            (emojiCode == ":(")
            content += " 🙁";
        else if
            (emojiCode == "<3")
            content += " ❤️";
        else if
            (emojiCode == ":D")
            content += " 😀 ";

        else if (emojiCode == "thumbsup")
            content += "👍 ";
        else
            content += " " + emojiCode;

    }
};

// ========================
//       CHAT CLASS (BASE)
// ========================
class Chat {
protected:
    vector<string> participants;
    vector<Message> messages;
    string chatName;
    ChatType type;

public:
    Chat() {
        participants = {};
        messages = {};
        chatName = "";
    }

    virtual ~Chat() = default;

    Chat(vector<string> users, string name) {
        participants = users;
        chatName = name;
    }

    ChatType getType() const {
        return type;
    }

    string getChatName() const {
        return chatName;
    }

    void addMessage(const Message& msg) {
        messages.push_back(msg);
    }

    bool deleteMessage(int index, const string& username) {
        if( index < 0 || index >= messages.size() || messages[index].getSender() != username )
        {
           return false;
        }
        else
        {
            messages.erase(messages.begin() + index);
            return true;
        }

    }

    bool isParticipant(const string& username) const { //added this as public method from groupChat class so WhatsApp can check it 
        for (const string& p : participants) {
            if (p == username) {
                return true;
            }
        }
        return false;
    }

    virtual void displayChat() const {
        cout << "chat name : " << chatName;
        for( const auto& msg : messages )
        {
            msg.display();
        }
    }

    vector<Message> searchMessages(string keyword) const {
        vector<Message> result;
        for (const auto& msg : messages) {
          if (msg.getContent().find(keyword)!= string::npos)
              {
                result.push_back(msg);
              }
            }
        return result;

    }

    void exportToFile(const string& filename) const {

      ofstream file(filename);
      if (!file.is_open())
       return;
      file << "Chat Name: " << chatName << endl;
      for (const auto& msg : messages)
                                      {
       file << msg.getSender()<< ": "<< msg.getContent()<< endl;
  }
      file.close();
    }
};

// ========================
//     PRIVATE CHAT CLASS
// ========================
class PrivateChat : public Chat {
private:
    string user1;
    string user2;

public:
    PrivateChat(string u1, string u2) {
        user1 = u1;
        user2 = u2;
        participants.push_back(u1);
        participants.push_back(u2);
        chatName = "Chat between " + u1 + " and " + u2;
        type = PRIVATE_CHAT;
    }


    void displayChat() const override {
         cout << "=== Private Chat: " << user1 << " & " << user2 << " ===" << endl;
         if (messages.empty()) {
            cout << "(no messages yet)" << endl;
            return;
    }
    for (int i = 0; i < (int)messages.size(); i++) {
            cout << "[" << i << "] ";
            messages[i].display();
        }
    }

    void showTypingIndicator(const string& username) const {
         cout << username << " is typing..." << endl;
    }
};

// ========================
//      GROUP CHAT CLASS
// ========================
class GroupChat : public Chat {
private:
    vector<string> admins;
    string description;

public:
     GroupChat(vector<string> users, string name, string creator)
    : Chat(users, name) {
        admins.push_back(creator);
        type = GROUP_CHAT;
    }

    void addAdmin(string newAdmin) {
        admins.push_back(newAdmin);
    }

    bool removeParticipant(const string& admin, const string& userToRemove) {
        if (isParticipant(userToRemove) && !isAdmin(userToRemove) ) {
            for (int i = 0; i < (int)participants.size(); i++) {
                if (participants[i] == userToRemove) {
                    participants.erase(participants.begin() + i);
                    return true;
                }
            }
        }
        return false;
    }

    bool isAdmin(string username) const {
        for (string admin: admins) {
            if (admin == username) {
                return true;
            }
        }
        return false;
    }

    bool isParticipant(string username) const {
        for (string participant: participants) {
            if (participant == username) {
                return true;
            }
        }
        return false;
    }

    bool setDescription(string desc) {
        if (desc.length() > 0 && desc.length() <= 1024) {
            description = desc;
            return true;
        }

        return false;
    }

    void displayChat() const override {
        cout << "===== " << chatName << " =====" << endl;
        cout << "Description: " << description << endl;
        cout << "Admins:" << endl;
        for (string admin: admins) {
            cout << "- " << admin << endl;
        }
        cout << "Participants:" << endl;
        for (string participant: participants) {
            cout << "- " << participant << endl;
        }

        cout << "Messages:" << endl;
        for (Message message: messages) {
            message.display();
        }
    }

    void sendJoinRequest(const string& username) {
        if (!isParticipant(username)) {
            cout << username << " requested to join " << chatName << "." << endl;
        } else {
            cout << username << " is already a member in " << chatName << "!" << endl;
        }
    }
};

// ========================
//    WHATSAPP APP CLASS
// ========================
class WhatsApp {
private:
    vector<User> users;
    vector<Chat*> chats;
    int currentUserIndex;

    int findUserIndex(string username) const {
      for (int i = 0; i < (int)users.size(); i++) {
            if (users[i].getUsername() == username) {
                return i;
            }
        }
        return -1;
    }

    bool isLoggedIn() const {
        return currentUserIndex != -1;
    }

    string getCurrentUsername() const {
        if (currentUserIndex != -1) {
            return users[currentUserIndex].getUsername();
        }
        return "";
    }

public:
    WhatsApp() : currentUserIndex(-1) {}

	~WhatsApp() { //destructor to prevent memory leaks
        for (Chat* c : chats) {
            delete c;
        }
    }

    void signUp() {
        string uname, pwd, phone;

        cout << "Enter username: ";
        cin >> uname;

        if (findUserIndex(uname) != -1) {
            cout << "Username already taken. Please choose another." << endl;
            return;
        }

        cout << "Enter password (at least 6 characters): ";
        cin >> pwd;

        if (pwd.length() < 6) {
            cout << "Password must be at least 6 characters long." << endl;
            return;
        }

        cout << "Enter phone number: ";
        cin >> phone;

        User newUser(uname, pwd, phone);
        users.push_back(newUser);

        cout << "Account created successfully! You can now log in." << endl;
    }

    void login() {
        string uname , pwd;
        cout << "Enter the username : ";
        cin >> uname;
        cout << "Enter the password : ";
        cin >> pwd;
        int index = findUserIndex(uname);
        if (index == -1)
        {
            cout << "User not found" << endl;
        }
        else if (!users[index].checkPassword(pwd))
        {
            cout << "Incorrect password" << endl;
        }
        else{
              currentUserIndex = index;
        cout << "Login successful, welcome back " << users[index].getUsername() << "!" << endl;
        }

    }

    void startPrivateChat() {
        string targetUsername;
        cout << "Enter username to chat with: ";
        cin >> targetUsername;

        int targetIndex = findUserIndex(targetUsername);

        if (targetIndex == -1) {
            cout << "User not found!\n";
            return;

        }

        Chat* newChat = new PrivateChat(users[currentUserIndex].getUsername(), users[targetIndex].getUsername());
        chats.push_back(newChat);

        cout << "Private chat started with " << targetUsername << "!\n";
    }

    void createGroup() {
        string groupName;
        cout << "Enter group name: ";
        cin >> groupName;

        vector<string> groupMembers;
        groupMembers.push_back(getCurrentUsername());

        string memberUsername;
        cout << "Enter member usernames (type 'done' to finish):\n";
        while (true) {
            cout << "Add member: ";
            cin >> memberUsername;
            if (memberUsername == "done") {
                break;
            }

            int idx = findUserIndex(memberUsername);
            if (idx != -1) {
                groupMembers.push_back(memberUsername);
                cout << memberUsername << " added to group.\n";
            }
            else {
                cout << "User not found!\n";
            }
        }

        if (groupMembers.size() < 2) {
            cout << "A group needs at least 2 participants. Group not created.\n";
            return;
        }

        Chat* newGroup = new GroupChat(groupMembers, groupName, getCurrentUsername());
        chats.push_back(newGroup);

        cout << "Group '" << groupName << "' created successfully!\n";
    }

    void manageGroup() {
        string groupName;
        cout << "Enter group name: ";
        cin >> groupName;
    
        GroupChat* group = nullptr;
        for (int i = 0; i < (int)chats.size(); i++) {
            if (chats[i]->getType() == GROUP_CHAT && chats[i]->getChatName() == groupName) {
                group = (GroupChat*)chats[i];
            }
        }
        
        if (group == nullptr) {
            cout << "Group not found!" << endl;
            return;
        }
        
        string currentUser = getCurrentUsername();

        if (!group->isAdmin(currentUser)) {
            cout << "Only admins can manage group!" << endl;
            return;
        }
    
        if (!group->isParticipant(currentUser)) {
            group->sendJoinRequest(currentUser);
            return;
        }
    
        cout << "\n1. Add Admin\n2. Remove Participant\n3. Set Description\nChoice: ";
        int choice;
        cin >> choice;
    
        if (choice == 1) {
            string newAdmin;
            cout << "Enter username: ";
            cin >> newAdmin;
            if (group->isParticipant(newAdmin)) {
                if (!group->isAdmin(newAdmin)) {
                    group->addAdmin(newAdmin);
                    cout << newAdmin << " is now an admin." << endl;
                } else {
                    cout << "User is already admin in this group!" << endl;
                }
            }
            else {
                cout << "User is not in this group!" << endl;
            }
        }
        else if (choice == 2) {
            string target;
            cout << "Enter username: ";
            cin >> target;
            if (group->removeParticipant(currentUser, target)) {
                cout << target << " removed from the group." << endl;
            } else {
                cout << "Could not remove user." << endl;
            }
        }
        else if (choice == 3) {
            string desc;
            cout << "Enter description: ";
            cin.ignore();
            getline(cin, desc);
            if (group->setDescription(desc)) {
                cout << "Description updated." << endl;                
            } else {
                cout << "Description must be between 1-1024 characters!";
            }
        }
    }

    void viewChats() const {
        string currentUser = getCurrentUsername();
        bool foundAny = false;
        int displayIndex = 1;

        for (size_t i = 0; i < chats.size(); ++i) {
            if (chats[i]->isParticipant(currentUser)) {
                if (!foundAny) {
                    cout << "\n--- YOUR CHATS ---\n";
                }
                cout << displayIndex << ". ";
                chats[i]->displayChat();
                displayIndex++;
                foundAny = true;
            }
        }

        if (!foundAny) {
            cout << "No chats available.\n";
        }
    }

    void logout() {
        if (!isLoggedIn()) {
            cout << " no user is current logged in " << endl;
        }
        currentUserIndex = -1;
    }
    void run() {
        while (true) {
            if (!isLoggedIn()) {
                cout << "\n1. Login\n2. Sign Up\n3. Exit\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1) login();
                else if (choice == 2) signUp();
                else if (choice == 3) break;
            }
            else {
                cout << "\n1. Start Private Chat\n2. Create Group\n3. Manage Group\n4. View Chats\n5. Logout\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1) startPrivateChat();
                else if (choice == 2) createGroup();
                else if (choice == 3) manageGroup();
                else if (choice == 4) viewChats();
                else if (choice == 5) logout();
            }
        }
    }
};

// ========================
//          MAIN
// ========================
int main() {
    WhatsApp whatsapp;
    whatsapp.run();
    return 0;
}
