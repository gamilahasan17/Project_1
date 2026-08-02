#define _CRT_SECURE_NO_WARNINGS  // Deprecation Warnings
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <windows.h>  //for emojies
#include<fstream>
using namespace std;

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

public:
    Chat() {
        participants = {};
        messages = {};
        chatName = "";
    }

    Chat(vector<string> users, string name) {
        participants = users;
        chatName = name;
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
    }

    void addAdmin(string newAdmin) {
        admins.push_back(newAdmin);
    }

    bool removeParticipant(const string& admin, const string& userToRemove) {
        if (isParticipant(userToRemove) && !isAdmin(userToRemove) && isAdmin(admin)) {
            for (int i = 0; i < participants.size(); i++) {
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

    void setDescription(string desc) {
        int description_lenght = sizeof(desc) / sizeof(desc[0]);
        if (description_lenght <= 1024 || description_lenght > 0) {
            description = desc;
        }
    }

    void displayChat() const override {
        cout << "===== " << chatName << " =====" << endl;
        cout << "Description: " << description << endl;
        cout << "Admins:" << endl;
        for (int i = 0; i < admins.size(); i++) {
            cout << "- " << admins[i] << endl;
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
        }

        cout << username << " is already a member in " << chatName << "!" << endl;
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

    }

public:
    WhatsApp() : currentUserIndex(-1) {}

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
        if (!users[index].checkPassword(pwd))
        {
            cout << "Incorrect password" << endl;
        }
        currentUserIndex = index;
        cout << "Login successful, welcome back" << users[index].getUsername() << "!" << endl;
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

        Chat* newGroup = new GroupChat(groupMembers, groupName, getCurrentUsername());
        chats.push_back(newGroup);

        cout << "Group '" << groupName << "' created successfully!\n";
    }
    void viewChats() const {
        if (chats.empty()) {
            cout << "No chats available.\n";
            return;
        }

        cout << "\n--- YOUR CHATS ---\n";
        for (size_t i = 0; i < chats.size(); ++i) {
            cout << i + 1 << ". ";
            chats[i]->displayChat();
        }
    }

    void logout() {
        // TODO: Implement logout
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
                cout << "\n1. Start Private Chat\n2. Create Group\n3. View Chats\n4. Logout\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1) startPrivateChat();
                else if (choice == 2) createGroup();
                else if (choice == 3) viewChats();
                else if (choice == 4) logout();
            }
        }
    }
};

// ========================
//          MAIN
// ========================
int main() {
    SetConsoleOutputCP(CP_UTF8); // to find emojis

    WhatsApp whatsapp;
    whatsapp.run();
    return 0;
}
