#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"
#include "map.h"
#include <memory>
#include <iostream>
#include "monsters.h"

class Game;

///////////////////////////////////////////////////////////////////////////
// Player is derived from actor class
///////////////////////////////////////////////////////////////////////////
class Player : public Actor {
public:
    Player(int x, int y, int hitPoints = 20, int armorPoints = 2, int strengthPoints = 2, int dexterityPoints = 2);
    void Move(char direction, const Map& map, const Game& game) override;

    // player statistics
    void regenerateHealth();
    void increaseArmorPoints(int amount) { armorPoints += amount; }
    void increaseStrengthPoints(int amount) { strengthPoints += amount; }
    void increaseDexterityPoints(int amount) { dexterityPoints += amount; }
    int GetLevel() const;
    void SetLevel(int level);
    
    // player attack functionality
    void Attack(Monster& monster, Game& game);
    void OnDeath(Game& game) override;
    bool HasAttacked() const { return attacked; }
    bool IsFinalBlow() const {  return currentTargetMonster && currentTargetMonster->IsDead(); }
    Monster* GetCurrentTargetMonster() const { return currentTargetMonster; }
    void SetTargetMonster(Monster* monster);
    void ClearTargetMonster();
    bool DidAttackHit() const { return attackedHit; }
    
    // inventory system
    void addItem(std::shared_ptr<GameObjects> item);
    void displayInventory() const;


private:
    int level;
    
    // attacking system
    bool attacked;
    bool finalBlow;
    Monster* currentTargetMonster;
    
    // inventory system
    std::vector<std::shared_ptr<GameObjects>> inventory;
};

#endif // PLAYER_H

