#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "Team.h"
#include "User.h"

class DatabaseManager {
private:
    vector<Team> teams;
    vector<User> users;

public:
    bool login(string u, string p, bool &adminFlag);

    void loadUsers();
    void loadTeams();
    void loadAdmins();

    void saveUsers();
    void saveTeams();

    bool isValidName(const string &str);

    bool safeInputInt(const string &prompt, int &value);
    int safeChoice();

    void viewTeams();
    void addTeam();
    void deleteTeam();
    void searchTeam();
    void editTeam();
    void countTeamsWithLessThan10Players();
    void findTeamWithMostWins();

    void viewUsers();
    void addUser();
    void deleteUser();

    void sortTeams();
    void showHelp();
};
