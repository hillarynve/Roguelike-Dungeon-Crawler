// updating changes - fixed - update

#ifndef MONSTERS_H
#define MONSTERS_H

#include "actor.h"
#include <vector>
#include <iostream>

class Player;
class Game;

///////////////////////////////////////////////////////////////////////////
// Base Monster Class
///////////////////////////////////////////////////////////////////////////
class Monster : public Actor {
public:
    Monster(int x, int y,  const std::string& name, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints);
    virtual void Move(char direction, const Map& map, const Game& game) override = 0;  // Corrected signature
    virtual char GetSymbol() const = 0; // Pure virtual function to get symbol
    
    void Attack(Player& player, Game& game);
    void CheckAndAttackPlayer(Player& player, Game& game); // New method
    
    void OnDeath(Game& game) override;
    void SetName(const std::string& name){ this->name = name; }
    std::string GetName() const { return name; }
    
    bool HasAttacked() const { return attacked; } // Method to check if monster has attacked
    bool IsFinalBlow() const { return finalBlow; } // Method to check if final blow was dealt
    void SetAttacked(bool value) { attacked = value; } // Method to set attacked status
    void SetFinalBlow(bool value) { finalBlow = value; } // Method to set final blow status
    bool DidAttackHit() const { return attackedHit; }
    

private:
    std::string name;
    bool attacked;
    bool finalBlow;
};

///////////////////////////////////////////////////////////////////////////
// Bogeyman Class
///////////////////////////////////////////////////////////////////////////
class Bogeyman : public Monster {
public:
    Bogeyman(int x, int y, const std::string& name, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints);
    void Move(char direction, const Map& map, const Game& game) override;  // Corrected signature
    char GetSymbol() const override { return 'B'; }
    char CalculateDirection(const Coordinate& playerPos) const;
};

///////////////////////////////////////////////////////////////////////////
// Dragon Class
///////////////////////////////////////////////////////////////////////////
class Dragon : public Monster {
public:
    Dragon(int x, int y, const std::string& name, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints);
    void Move(char direction, const Map& map, const Game& game) override;  // Corrected signature
    char GetSymbol() const override { return 'D'; }
    void RegenerateHealth();
};

///////////////////////////////////////////////////////////////////////////
// Goblin Class
///////////////////////////////////////////////////////////////////////////
class Goblin : public Monster {
public:
    Goblin(int x, int y, const std::string& name, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints);
    void Move(char direction, const Map& map, const Game& game) override;  // Corrected signature
    char CalculateDirection(const Coordinate& playerPos) const;
    
    char GetSymbol() const override { return 'G'; }
    // unused Goblin Recursion Movement Attempt
//    void goblinmove(int r, int c, Map* m, int maxSteps);
//    bool reachesPlayer(int g_r, int g_c, int p_r, int p_c, std::vector<std::pair<int, int>>& visited, std::vector<std::pair<int, int>>& bestPath, int maxSteps, Map* m);

private:
    int posRow_;
    int posCol_;
};

///////////////////////////////////////////////////////////////////////////
// Snakewoman Class
///////////////////////////////////////////////////////////////////////////
class Snakewoman : public Monster {
public:
    Snakewoman(int x, int y, const std::string& name, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints);
    void Move(char direction, const Map& map, const Game& game) override;  // Corrected signature
    char GetSymbol() const override { return 'S'; }
    char CalculateDirection(const Coordinate& playerPos) const;
};

#endif // MONSTERS_H

