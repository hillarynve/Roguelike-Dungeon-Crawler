#ifndef WEAPONS_H
#define WEAPONS_H

#include "gameobjects.h"

///////////////////////////////////////////////////////////////////////////
// weapons class derived from gameObjects
///////////////////////////////////////////////////////////////////////////

class Weapons : public GameObjects {
public:
    Weapons(const std::string& name, char symbol, const std::string& action, int dexterityBonus, int damageAmount);
    
    std::string GetAction() const;
    int GetDexterityBonus() const;
    int GetDamageAmount() const;

private:
    std::string action;
    int dexterityBonus;
    int damageAmount;
};

// Specific weapon classes
class Mace : public Weapons {
public:
    Mace();
};

class ShortSword : public Weapons {
public:
    ShortSword();
};

class LongSword : public Weapons {
public:
    LongSword();
};

class MagicAxe : public Weapons {
public:
    MagicAxe();
};

class MagicFangsOfSleep : public Weapons {
public:
    MagicFangsOfSleep();
};

#endif // WEAPONS_H
