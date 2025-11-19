#include <iostream>
#include "DatabaseManager.h"
using namespace std;

int main() {
    DatabaseManager db;

    try {
        db.loadUsers();
        db.loadTeams();
        db.loadAdmins();
    } catch (const exception &e) {
        cerr << e.what() << endl;
        return 1;
    }

    string login, pass;
    bool isAdmin = false;

    while (true) {
        cout << "Login: ";
        cin >> login;

        cout << "Password: ";
        cin >> pass;

        if (db.login(login, pass, isAdmin)) {
            break;
        }

        cout << "Invalid login or password! Try again.\n";
    }

    cout << "Login successful!\n";
    cout << "Welcome, " << login << "! You are "
         << (isAdmin ? "Administrator" : "User") << ".\n";

    int choice;
    if (isAdmin) {
        do {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. View teams\n2. Add team\n3. Delete team\n4. Search team\n5. Edit team\n";
            cout << "6. Count teams with <10 players\n7. Find team with most wins\n";
            cout << "8. View users\n9. Add user\n10. Delete user\n11. Sort teams\n12. Help\n0. Exit\nChoice: ";

            choice = db.safeChoice();

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

            choice = db.safeChoice();

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
