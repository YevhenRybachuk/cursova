#pragma once
#include <string>

using namespace std;

class User {
private:
    string username;
    string password;
    bool isAdmin;

public:
    User();
    User(string u, string p, bool a);

    void setAdmin(bool a);
    string getPassword() const;
    string getUsername() const;

    void loadFromString(const string &data);
    string toString() const;

    bool checkPassword(const string &u, const string &p) const;
    bool admin() const;
};
