#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include "Entity.h"
using namespace std;

class Team : public Entity {
private:
    string name, city;
    int gamesPlayed, wins, losses, draws, playersCount;

public:
    Team();
    Team(string n, string c, int g, int w, int l, int d, int p);

    void display() const override;

    void loadFromString(const string &data) override;
    string toString() const override;

    int getWins() const;
    int getPlayersCount() const;
    int getGamesPlayed() const;
    int getLosses() const;
    int getDraws() const;
    string getName() const;
    string getCity() const;

    void setName(const string &n);
    void setCity(const string &c);
    void setGames(int g);
    void setWins(int w);
    void setLosses(int l);
    void setDraws(int d);
    void setPlayers(int p);
};
