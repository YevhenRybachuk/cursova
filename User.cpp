#include "User.h"

User::User() : isAdmin(false) {}

User::User(string u, string p, bool a)
    : username(u), password(p), isAdmin(a) {}

void User::loadFromString(const string &data) {
    size_t pos = data.find(':');
    username = data.substr(0, pos);
    password = data.substr(pos + 1);
    isAdmin = (username == "admin");
}

string User::toString() const {
    return username + ":" + password;
}

bool User::checkPassword(const string &u, const string &p) const {
    return username == u && password == p;
}

bool User::admin() const {
    return isAdmin;
}

string User::getUsername() const {
    return username;
}
