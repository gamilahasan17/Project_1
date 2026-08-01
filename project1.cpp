#define _CRT_SECURE_NO_WARNINGS  // Deprecation Warnings
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <windows.h>  //for emojies

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
        username = "";
        password = "";
        phoneNumber = "";
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
        // TODO: Implement default constructor
    }

    Chat(vector<string> users, string name) {
        // TODO: Implement parameterized constructor
    }

    void addMessage(const Message& msg) {
        // TODO: Implement message addition
    }

    bool deleteMessage(int index, const string& username) {
        // TODO: Implement message deletion
        return false;
    }

    virtual void displayChat() const {
        // TODO: Implement chat display
    }

    vector<Message> searchMessages(string keyword) const {
        // TODO: Implement message search
        return {};
    }

    void exportToFile(const string& filename) const {
        // TODO: Implement export to file
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
    GroupChat(vector<string> users, string name, string creator) {
        // TODO: Implement constructor
    }

    void addAdmin(string newAdmin) {
        // TODO: Implement add admin
    }

    bool removeParticipant(const string& admin, const string& userToRemove) {
        // TODO: Implement remove participant
        return false;
    }

    bool isAdmin(string username) const {
        // TODO: Implement admin check
        return false;
    }

    bool isParticipant(string username) const {
        // TODO: Implement participant check
        return false;
    }

    void setDescription(string desc) {
        // TODO: Implement set description
    }

    void displayChat() const override {
        // TODO: Implement group chat display
    }

    void sendJoinRequest(const string& username) {
        // TODO: Implement join request
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
        // TODO: Implement login check
        return false;
    }

    string getCurrentUsername() const {
        // TODO: Implement get current user
        return "";
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
        // TODO: Implement user login
    }

    void startPrivateChat() {
        // TODO: Implement private chat creation
    }

    void createGroup() {
        // TODO: Implement group creation
    }

    void viewChats() const {
        // TODO: Implement chat viewing
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
