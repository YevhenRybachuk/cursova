#include "Team.h"

Team::Team()
    : gamesPlayed(0), wins(0), losses(0), draws(0), playersCount(0) {}

Team::Team(string n, string c, int g, int w, int l, int d, int p)
    : name(n), city(c), gamesPlayed(g), wins(w), losses(l), draws(d), playersCount(p) {}

void Team::display() const {
    cout << name << " (" << city << ") | Games: " << gamesPlayed
         << " | Wins: " << wins << " | Losses: " << losses
         << " | Draws: " << draws << " | Players: " << playersCount << endl;
}

void Team::loadFromString(const string &data) {
    stringstream ss(data);
    getline(ss, name, ',');
    getline(ss, city, ',');
    ss >> gamesPlayed; ss.ignore();
    ss >> wins; ss.ignore();
    ss >> losses; ss.ignore();
    ss >> draws; ss.ignore();
    ss >> playersCount;
}

string Team::toString() const {
    stringstream ss;
    ss << name << "," << city << "," << gamesPlayed << "," << wins
       << "," << losses << "," << draws << "," << playersCount;
    return ss.str();
}

int Team::getWins() const { return wins; }
int Team::getPlayersCount() const { return playersCount; }
int Team::getGamesPlayed() const { return gamesPlayed; }
int Team::getLosses() const { return losses; }
int Team::getDraws() const { return draws; }
string Team::getName() const { return name; }
string Team::getCity() const { return city; }

void Team::setName(const string &n) { name = n; }
void Team::setCity(const string &c) { city = c; }
void Team::setGames(int g) { gamesPlayed = g; }
void Team::setWins(int w) { wins = w; }
void Team::setLosses(int l) { losses = l; }
void Team::setDraws(int d) { draws = d; }
void Team::setPlayers(int p) { playersCount = p; }
