#include "DatabaseManager.h"

#include <iostream>
#include <limits>

using namespace std;

bool DatabaseManager::isValidName(const string &str) {
    if (str.empty())
        return false;

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

void DatabaseManager::loadAdmins() {
    ifstream file("admins.txt");
    if (!file)
        throw runtime_error("Failed to open admins.txt");

    string line;
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos == string::npos)
            continue;

        string adminUsername = line.substr(0, pos);
        string adminPassword = line.substr(pos + 1);

        for (auto &user : users) {
            if (user.getUsername() == adminUsername &&
                user.checkPassword(adminUsername, adminPassword)) {
                user.setAdmin(true);
                break;
            }
        }
    }
}

void DatabaseManager::loadUsers() {
    ifstream file("users.txt");
    if (!file)
        throw runtime_error("Failed to open users.txt");

    string line;
    while (getline(file, line)) {
        User u;
        u.loadFromString(line);
        users.push_back(u);
    }
}

void DatabaseManager::loadTeams() {
    ifstream file("teams.csv");
    if (!file)
        throw runtime_error("Failed to open teams.csv");

    string line;
    while (getline(file, line)) {
        Team t;
        t.loadFromString(line);
        teams.push_back(t);
    }
}

void DatabaseManager::saveTeams() {
    ofstream file("teams.csv", ios::trunc);
    for (auto &t : teams)
        file << t.toString() << "\n";
}

void DatabaseManager::saveUsers() {
    ofstream file("users.txt");
    for (auto &u : users)
        file << u.toString() << "\n";
}

bool DatabaseManager::login(string u, string p, bool &adminFlag) {
    for (auto &usr : users) {
        if (usr.checkPassword(u, p)) {
            adminFlag = usr.admin();
            return true;
        }
    }
    return false;
}

bool DatabaseManager::safeInputInt(const string &prompt, int &value) {
    cout << prompt;
    while (true) {
        if (cin >> value) {
            if (value >= 0)
                return true;
            else
                cout << "Number must be positive! Try again: ";
        } else {
            cout << "Invalid input! Enter a number: ";
            cin.clear();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int DatabaseManager::safeChoice() {
    int x;
    while (true) {
        if (cin >> x) {
            if (x >= 0)
                return x;
            cout << "Choice must be >= 0: ";
        } else {
            cout << "Invalid number: ";
            cin.clear();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void DatabaseManager::viewTeams() {
    for (auto &t : teams)
        t.display();
}

void DatabaseManager::addTeam() {
    string name, city;
    int g, w, l, d, p;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    do {
        cout << "Enter team name: ";
        getline(cin, name);
        if (!isValidName(name))
            cout << "Invalid name!\n";
    } while (!isValidName(name));

    do {
        cout << "Enter city: ";
        getline(cin, city);
        if (!isValidName(city))
            cout << "Invalid city!\n";
    } while (!isValidName(city));

    safeInputInt("Games played: ", g);
    safeInputInt("Wins: ", w);
    safeInputInt("Losses: ", l);
    safeInputInt("Draws: ", d);
    safeInputInt("Players count: ", p);

    if (w + l + d != g) {
        cout << "Error: Wins + Losses + Draws must equal Games Played!\n";
        return;
    }

    teams.push_back(Team(name, city, g, w, l, d, p));
    saveTeams();
    cout << "Team added!\n";
}

void DatabaseManager::deleteTeam() {
    string name;
    cout << "Enter team name to delete: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    for (auto it = teams.begin(); it != teams.end(); ++it) {
        if (it->getName() == name) {
            teams.erase(it);
            saveTeams();
            cout << "Team deleted!\n";
            return;
        }
    }
    cout << "Team not found.\n";
}

void DatabaseManager::searchTeam() {
    string name;
    cout << "Enter team name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    for (auto &t : teams) {
        if (t.getName() == name) {
            t.display();
            return;
        }
    }
    cout << "Team not found.\n";
}

void DatabaseManager::editTeam() {
    string name;
    cout << "Enter team name to edit: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    for (auto &t : teams) {
        if (t.getName() == name) {
            string newName, newCity;
            int g, w, l, d, p;

            cout << "Editing team: " << name << endl;

            do {
                cout << "Enter NEW team name: ";
                getline(cin, newName);
                if (newName.empty())
                    cout << "Cannot be empty!\n";
            } while (!isValidName(newName));

            do {
                cout << "Enter NEW city: ";
                getline(cin, newCity);
                if (newCity.empty())
                    cout << "Cannot be empty!\n";
            } while (!isValidName(newCity));

            cout << "Enter NEW games played: ";
            g = safeChoice();

            cout << "Enter NEW wins: ";
            w = safeChoice();

            cout << "Enter NEW losses: ";
            l = safeChoice();

            cout << "Enter NEW draws: ";
            d = safeChoice();

            cout << "Enter NEW players count: ";
            p = safeChoice();

            if (w + l + d != g) {
                cout << "Error: Wins + Losses + Draws != Games Played!\n";
                return;
            }

            t.setName(newName);
            t.setCity(newCity);
            t.setGames(g);
            t.setWins(w);
            t.setLosses(l);
            t.setDraws(d);
            t.setPlayers(p);

            saveTeams();
            cout << "Team updated!\n";
            return;
        }
    }
    cout << "Team not found.\n";
}

void DatabaseManager::countTeamsWithLessThan10Players() {
    int count = 0;
    for (auto &t : teams)
        if (t.getPlayersCount() < 10)
            count++;
    cout << "Teams with <10 players: " << count << endl;
}

void DatabaseManager::findTeamWithMostWins() {
    if (teams.empty())
        return;

    Team best = teams[0];
    for (auto &t : teams)
        if (t.getWins() > best.getWins())
            best = t;

    cout << "Team with most wins:\n";
    best.display();
}

void DatabaseManager::viewUsers() {
    for (auto &u : users)
        cout << u.getUsername() << (u.admin() ? " (Admin)" : " (User)") << endl;
}

void DatabaseManager::addUser() {
    string u, p;
    cout << "Username: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;

    users.push_back(User(u, p, false));
    saveUsers();
    cout << "User added!\n";
}

void DatabaseManager::deleteUser() {
    string u;
    cout << "Enter username: ";
    cin >> u;

    if (u == "admin") {
        cout << "Cannot delete admin.\n";
        return;
    }

    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->getUsername() == u) {
            users.erase(it);
            saveUsers();
            cout << "User deleted!\n";
            return;
        }
    }
    cout << "User not found.\n";
}

void DatabaseManager::sortTeams() {
    int choice;
    cout << "Sort by:\n"
         << "1. Name\n"
         << "2. City\n"
         << "3. Games\n"
         << "4. Wins\n"
         << "5. Losses\n"
         << "6. Draws\n"
         << "7. Players\n"
         << "Choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            sort(teams.begin(), teams.end(),
                 [](Team &a, Team &b) { return a.getName() < b.getName(); });
            break;
        case 2:
            sort(teams.begin(), teams.end(),
                 [](Team &a, Team &b) { return a.getCity() < b.getCity(); });
            break;
        case 3:
            sort(teams.begin(), teams.end(),
                 [](Team &a, Team &b) { return a.getGamesPlayed() > b.getGamesPlayed(); });
            break;
        case 4:
            sort(teams.begin(), teams.end(),
                 [](Team &a, Team &b) { return a.getWins() > b.getWins(); });
            break;
        case 5:
            sort(teams.begin(), teams.end(),
                 [](Team &a, Team &b) { return a.getLosses() > b.getLosses(); });
            break;
        case 6:
            sort(teams.begin(), teams.end(),
                 [](Team &a, Team &b) { return a.getDraws() > b.getDraws(); });
            break;
        case 7:
            sort(teams.begin(), teams.end(),
                 [](Team &a, Team &b) { return a.getPlayersCount() > b.getPlayersCount(); });
            break;
        default:
            cout << "Invalid choice.\n";
            return;
    }

    cout << "Teams sorted.\n";
    viewTeams();
}

void DatabaseManager::showHelp() {
    cout << "\n=== USER MANUAL ===\n";

    cout << "\n1. Program Purpose\n\n"
         << "This program allows a regular user to view and "
         << "search information about football teams.\n"
         << "The user can also sort teams and view basic "
         << "statistics.\n"
         << "Modifying teams or managing users is not "
         << "available for regular users.\n";

    cout << "\n2. Data Input Rules\n\n"
         << "Text Input (team name, city):\n"
         << "- Only letters, spaces, and hyphens are allowed.\n"
         << "- The field cannot be empty.\n\n"
         << "Valid examples:\n"
         << "Tigers, Red Wolves, New-York\n\n"
         << "Invalid examples:\n"
         << "Tiger123, @Team, !!!\n\n"
         << "Numeric Input:\n"
         << "- All numeric fields must be whole numbers >= 0.\n"
         << "- Includes: games played, wins, losses, draws, number of players.\n"
         << "- If text is entered instead of a number, the program will request input again.\n\n"
         << "Important rule:\n"
         << "Wins + Losses + Draws must equal Games Played.\n";

    cout << "\n3. Menu Commands (Regular User)\n\n"
         << "1. View teams\n"
         << "2. Search team\n"
         << "3. Count teams with less than 10 players\n"
         << "4. Find team with most wins\n"
         << "5. Sort teams\n"
         << "6. Help\n"
         << "0. Exit\n\n"
         << "Regular users cannot modify teams or manage user accounts.\n\n";
}
