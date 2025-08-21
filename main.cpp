#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

class ISerializable {
public:
    virtual void loadFromString(const string &data) = 0;
    virtual string toString() const = 0;
    virtual ~ISerializable() {}
};

class Entity : public ISerializable {
protected:
    int id;
public:
    virtual void display() const = 0;
    virtual ~Entity() {}
};

class Team : public Entity {
private:
    string name, city;
    int gamesPlayed, wins, losses, draws, playersCount;
public:
    Team() : gamesPlayed(0), wins(0), losses(0), draws(0), playersCount(0) {}
    Team(string n, string c, int g, int w, int l, int d, int p)
        : name(n), city(c), gamesPlayed(g), wins(w), losses(l), draws(d), playersCount(p) {}

    void display() const override {
        cout << name << " (" << city << ") | Games: " << gamesPlayed
             << " | Wins: " << wins << " | Losses: " << losses
             << " | Draws: " << draws << " | Players: " << playersCount << endl;
    }

    void loadFromString(const string &data) override {
        stringstream ss(data);
        getline(ss, name, ',');
        getline(ss, city, ',');
        ss >> gamesPlayed; ss.ignore();
        ss >> wins; ss.ignore();
        ss >> losses; ss.ignore();
        ss >> draws; ss.ignore();
        ss >> playersCount;
    }

    string toString() const override {
        stringstream ss;
        ss << name << "," << city << "," << gamesPlayed << "," << wins
           << "," << losses << "," << draws << "," << playersCount;
        return ss.str();
    }

    int getWins() const { return wins; }
    int getPlayersCount() const { return playersCount; }
    int getGamesPlayed() const { return gamesPlayed; }
    int getLosses() const { return losses; }
    int getDraws() const { return draws; }
    string getName() const { return name; }
    string getCity() const { return city; }

    void setName(const string &n) { name = n; }
    void setCity(const string &c) { city = c; }
    void setGames(int g) { gamesPlayed = g; }
    void setWins(int w) { wins = w; }
    void setLosses(int l) { losses = l; }
    void setDraws(int d) { draws = d; }
    void setPlayers(int p) { playersCount = p; }
};

class User : public ISerializable {
private:
    string username, password;
    bool isAdmin;
public:
    User() : isAdmin(false) {}
    User(string u, string p, bool a) : username(u), password(p), isAdmin(a) {}

    void loadFromString(const string &data) override {
        size_t pos = data.find(':');
        username = data.substr(0, pos);
        password = data.substr(pos + 1);
        isAdmin = (username == "admin");
    }

    string toString() const override {
        return username + ":" + password;
    }

    bool checkPassword(const string &u, const string &p) const {
        return username == u && password == p;
    }

    bool admin() const { return isAdmin; }
    string getUsername() const { return username; }
};

class DatabaseManager {
private:
    vector<Team> teams;
    vector<User> users;
public:

    int safeInput(const string &prompt) {
        string input;
        int value;
        while (true) {
            cout << prompt;
            getline(cin, input);
            stringstream ss(input);
            if (ss >> value && ss.eof() && value >= 0) {
                return value;
            } else {
                cout << "Invalid input! Please enter a non-negative whole number.\n";
            }
        }
    }

    bool isValidName(const string &str) {
        if (str.empty()) return false;

        bool hasLetter = false;
        for (char c : str) {
            if (isalpha(static_cast<unsigned char>(c))) {
                hasLetter = true;
            } else if (!(c == ' ' || c == '-')) {
                return false;
            }
        }
        return hasLetter;
    }


    void loadUsers() {
        ifstream file("users.txt");
        if (!file) throw runtime_error("Failed to open users.txt");
        string line;
        while (getline(file, line)) {
            User u;
            u.loadFromString(line);
            users.push_back(u);
        }
    }

    void loadTeams() {
        ifstream file("teams.csv");
        if (!file) throw runtime_error("Failed to open teams.csv");
        string line;
        while (getline(file, line)) {
            Team t;
            t.loadFromString(line);
            teams.push_back(t);
        }
    }

    void saveTeams() {
        ofstream file("teams.csv", ios::out | ios::trunc);
        for (auto &t : teams) {
            file << t.toString() << "\n";
        }
    }

    void saveUsers() {
        ofstream file("users.txt");
        for (auto &u : users) {
            file << u.toString() << "\n";
        }
    }

    bool login(string u, string p, bool &adminFlag) {
        for (auto &usr : users) {
            if (usr.checkPassword(u, p)) {
                adminFlag = usr.admin();
                return true;
            }
        }
        return false;
    }

    void viewTeams() {
        for (auto &t : teams) t.display();
    }

    void addTeam() {
        string name, city;

        cin.ignore();

        do {
            cout << "Enter team name (letters, spaces, hyphens only): ";
            getline(cin, name);
            if (!isValidName(name)) cout << "Invalid name! Try again.\n";
        } while (!isValidName(name));

        do {
            cout << "Enter city (letters, spaces, hyphens only): ";
            getline(cin, city);
            if (!isValidName(city)) cout << "Invalid city name! Try again.\n";
        } while (!isValidName(city));

        int g = safeInput("Games played: ");
        int w = safeInput("Wins: ");
        int l = safeInput("Losses: ");
        int d = safeInput("Draws: ");
        int p = safeInput("Players count: ");

        if (w + l + d != g) {
            cout << "Error: Wins + Losses + Draws must equal Games Played!\n";
            return;
        }

        teams.push_back(Team(name, city, g, w, l, d, p));
        saveTeams();
        cout << "Team added and saved!\n";
    }

    void deleteTeam() {
        string name;
        cout << "Enter team name to delete: ";
        cin.ignore(); getline(cin, name);
        for (auto it = teams.begin(); it != teams.end(); ++it) {
            if (it->getName() == name) {
                teams.erase(it);
                saveTeams();
                cout << "Team deleted and saved!\n";
                return;
            }
        }
        cout << "Team not found.\n";
    }

    void searchTeam() {
        string name;
        cout << "Enter team name to search: ";
        cin.ignore(); getline(cin, name);
        for (auto &t : teams) {
            if (t.getName() == name) {
                t.display();
                return;
            }
        }
        cout << "Team not found.\n";
    }

    void editTeam() {
    string name;
    cout << "Enter team name to edit: ";
    cin.ignore();
    getline(cin, name);

    for (auto &t : teams) {
        if (t.getName() == name) {
            string newName, newCity;
            int g, w, l, d, p;

            cout << "Editing team: " << name << endl;

            do {
                cout << "Enter new name (leave empty to keep current): ";
                getline(cin, newName);
                if (!newName.empty() && !isValidName(newName)) {
                    cout << "Invalid name! Try again.\n";
                }
            } while (!newName.empty() && !isValidName(newName));

            do {
                cout << "Enter new city (leave empty to keep current): ";
                getline(cin, newCity);
                if (!newCity.empty() && !isValidName(newCity)) {
                    cout << "Invalid city name! Try again.\n";
                }
            } while (!newCity.empty() && !isValidName(newCity));

            cout << "Games played (-1 to keep current): "; cin >> g;
            cout << "Wins (-1 to keep current): "; cin >> w;
            cout << "Losses (-1 to keep current): "; cin >> l;
            cout << "Draws (-1 to keep current): "; cin >> d;
            cout << "Players count (-1 to keep current): "; cin >> p;

            if (g < 0) g = t.getGamesPlayed();
            if (w < 0) w = t.getWins();
            if (l < 0) l = t.getLosses();
            if (d < 0) d = t.getDraws();
            if (p < 0) p = t.getPlayersCount();

            if (w + l + d != g) {
                cout << "Error: Wins + Losses + Draws must equal Games Played!\n";
                return;
            }

            if (!newName.empty()) t.setName(newName);
            if (!newCity.empty()) t.setCity(newCity);
            t.setGames(g);
            t.setWins(w);
            t.setLosses(l);
            t.setDraws(d);
            t.setPlayers(p);

            saveTeams();
            cout << "Team updated and saved!\n";
            return;
        }
    }
    cout << "Team not found.\n";
}



    void countTeamsWithLessThan10Players() {
        int count = 0;
        for (auto &t : teams) if (t.getPlayersCount() < 10) count++;
        cout << "Number of teams with less than 10 players: " << count << endl;
    }

    void findTeamWithMostWins() {
        if (teams.empty()) return;
        Team best = teams[0];
        for (auto &t : teams) if (t.getWins() > best.getWins()) best = t;
        cout << "Team with the most wins:\n";
        best.display();
    }

    void viewUsers() {
        for (auto &u : users) cout << u.getUsername() << (u.admin() ? " (Admin)" : " (User)") << endl;
    }

    void addUser() {
        string u, p;
        cout << "Enter username: "; cin >> u;
        cout << "Enter password: "; cin >> p;
        users.push_back(User(u, p, false));
        saveUsers();
        cout << "User added and saved!\n";
    }

    void deleteUser() {
        string u;
        cout << "Enter username to delete: "; cin >> u;
        if (u == "admin") { cout << "You cannot delete the admin.\n"; return; }
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->getUsername() == u) {
                users.erase(it);
                saveUsers();
                cout << "User deleted and saved!\n";
                return;
            }
        }
        cout << "User not found.\n";
    }

    void sortTeams() {
        int choice;
        cout << "Sort teams by:\n1. Name\n2. City\n3. Games played\n4. Wins\n5. Losses\n6. Draws\n7. Players count\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1: sort(teams.begin(), teams.end(), [](Team &a, Team &b){ return a.getName() < b.getName(); }); break;
            case 2: sort(teams.begin(), teams.end(), [](Team &a, Team &b){ return a.getCity() < b.getCity(); }); break;
            case 3: sort(teams.begin(), teams.end(), [](Team &a, Team &b){ return a.getGamesPlayed() > b.getGamesPlayed(); }); break;
            case 4: sort(teams.begin(), teams.end(), [](Team &a, Team &b){ return a.getWins() > b.getWins(); }); break;
            case 5: sort(teams.begin(), teams.end(), [](Team &a, Team &b){ return a.getLosses() > b.getLosses(); }); break;
            case 6: sort(teams.begin(), teams.end(), [](Team &a, Team &b){ return a.getDraws() > b.getDraws(); }); break;
            case 7: sort(teams.begin(), teams.end(), [](Team &a, Team &b){ return a.getPlayersCount() > b.getPlayersCount(); }); break;
            default: cout << "Invalid choice.\n"; return;
        }
        cout << "Teams sorted:\n";
        viewTeams();
    }

    void showHelp() {
        cout << "\n=== Help ===\n";
        cout << "This program allows you to work with football teams.\n";
        cout << "You can look at teams, search for a team, and see statistics.\n";
        cout << "Some actions are available only to special users with more rights.\n\n";

        cout << "How to enter information:\n";
        cout << "- Team name and city: write them as normal words (for example: Tigers, London).\n";
        cout << "- Numbers like games played, wins, losses, draws and players must be written as whole numbers (for example: 12).\n";
        cout << "- When changing a team, if you do not want to change some value, just type -1 or press Enter to leave it the same.\n\n";

        cout << "What each menu item means:\n";
        cout << " 1. View teams – show the list of all teams.\n";
        cout << " 2. Add team – create a new team (only for special users).\n";
        cout << " 3. Delete team – remove a team (only for special users).\n";
        cout << " 4. Search team – find a team by its name.\n";
        cout << " 5. Edit team – change team information (only for special users).\n";
        cout << " 6. Count teams with less than 10 players – see how many small teams exist.\n";
        cout << " 7. Find team with the most wins – see which team is the best.\n";
        cout << " 8. View users – show the list of people who can use the program (only for special users).\n";
        cout << " 9. Add user – allow a new person to use the program (only for special users).\n";
        cout << "10. Delete user – remove a person from the program (only for special users).\n";
        cout << "11. Sort teams – arrange teams by name, city, or results.\n";
        cout << "12. Help – show these instructions.\n";
        cout << " 0. Exit – close the program.\n";
        cout << "=====================================\n\n";
    }


};

int main() {
    DatabaseManager db;
    try {
        db.loadUsers();
        db.loadTeams();
    } catch (const exception &e) {
        cerr << e.what() << endl;
        return 1;
    }

    string login, pass;
    bool isAdmin = false;
    cout << "Login: "; cin >> login;
    cout << "Password: "; cin >> pass;

    if (!db.login(login, pass, isAdmin)) {
        cout << "Invalid login or password!\n";
        return 0;
    }

    cout << "Welcome, " << login << "! You are " << (isAdmin ? "Administrator" : "User") << ".\n";

    int choice;
    if (isAdmin) {
        do {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. View teams\n2. Add team\n3. Delete team\n4. Search team\n5. Edit team\n";
            cout << "6. Count teams with <10 players\n7. Find team with most wins\n";
            cout << "8. View users\n9. Add user\n10. Delete user\n11. Sort teams\n12. Help\n0. Exit\nChoice: ";
            cin >> choice;
            switch (choice) {
                case 1: db.viewTeams(); break;
                case 2: db.addTeam(); break;
                case 3: db.deleteTeam(); break;
                case 4: db.searchTeam(); break;
                case 5: db.editTeam(); break;
                case 6: db.countTeamsWithLessThan10Players(); break;
                case 7: db.findTeamWithMostWins(); break;
                case 8: db.viewUsers(); break;
                case 9: db.addUser(); break;
                case 10: db.deleteUser(); break;
                case 11: db.sortTeams(); break;
                case 12: db.showHelp(); break;
                case 0: cout << "Exiting...\n"; break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 0);
    } else {
        do {
            cout << "\n--- User Menu ---\n";
            cout << "1. View teams\n2. Search team\n3. Count teams with <10 players\n4. Find team with most wins\n5. Sort teams\n6. Help\n0. Exit\nChoice: ";
            cin >> choice;
            switch (choice) {
                case 1: db.viewTeams(); break;
                case 2: db.searchTeam(); break;
                case 3: db.countTeamsWithLessThan10Players(); break;
                case 4: db.findTeamWithMostWins(); break;
                case 5: db.sortTeams(); break;
                case 6: db.showHelp(); break;
                case 0: cout << "Exiting...\n"; break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 0);
    }

    return 0;
}
