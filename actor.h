//
//  actor.hpp
//
//  Created by Hillary Nguyen on 5/22/24.
//

#ifndef ACTOR_H
#define ACTOR_H

#include "coordinate.h"
#include "weapons.h"
#include "utilities.h"
#include <memory>

class Game;
class Map;

///////////////////////////////////////////////////////////////////////////
// Actor is the base class for Monster and Player
///////////////////////////////////////////////////////////////////////////

class Actor {
public:
    Actor(int x, int y, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints);
    virtual ~Actor() = default;
   
    // movement
    virtual void Move(char direction, const Map& map, const Game& game) = 0;
    Coordinate GetPosition() const;
    void SetPosition(int x, int y);
    
    // Getters and setters for the stat characteristics
    int GetHitPoints() const;
    void SetHitPoints(int hitPoints);
    int GetArmorPoints() const;
    void SetArmorPoints(int armorPoints);
    int GetStrengthPoints() const;
    void SetStrengthPoints(int strengthPoints);
    int GetDexterityPoints() const;
    void SetDexterityPoints(int dexterityPoints);

    // weapons
    std::shared_ptr<class Weapons> GetWeapon() const;
    void SetWeapon(std::shared_ptr<class Weapons> weapon);

    // attack functionality
    bool IsDead() const;
    virtual void OnDeath(Game& game) = 0;  
    virtual void Attack(Actor& target, Game& game);
    
protected:
    // statistics members
    Coordinate position;
    int hitPoints;
    int armorPoints;
    int strengthPoints;
    int dexterityPoints;
    std::shared_ptr<class Weapons> weapon;
    
    // attack members
    bool attacked;
    bool attackedHit;
    bool finalBlow;
};

#endif // ACTOR_H

