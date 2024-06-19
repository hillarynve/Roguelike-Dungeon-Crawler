#include "actor.h"

// constructor
Actor::Actor(int x, int y, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints)
    : position(x, y), hitPoints(hitPoints), armorPoints(armorPoints),
      strengthPoints(strengthPoints), dexterityPoints(dexterityPoints), weapon(nullptr) {}

// actor coordinates
Coordinate Actor::GetPosition() const {
    return position;
}

void Actor::SetPosition(int x, int y) {
    position.SetX(x);
    position.SetY(y);
}

// actor statistics
int Actor::GetHitPoints() const {
    return hitPoints;
}

void Actor::SetHitPoints(int hitPoints) {
    this->hitPoints = hitPoints;
}

int Actor::GetArmorPoints() const {
    return armorPoints;
}

void Actor::SetArmorPoints(int armorPoints) {
    this->armorPoints = armorPoints;
}

int Actor::GetStrengthPoints() const {
    return strengthPoints;
}

void Actor::SetStrengthPoints(int strengthPoints) {
    this->strengthPoints = strengthPoints;
}

int Actor::GetDexterityPoints() const {
    return dexterityPoints;
}

void Actor::SetDexterityPoints(int dexterityPoints) {
    this->dexterityPoints = dexterityPoints;
}

// actor weapon/item
std::shared_ptr<Weapons> Actor::GetWeapon() const {
    return weapon;
}

void Actor::SetWeapon(std::shared_ptr<Weapons> weapon) {
    this->weapon = weapon;
}

bool Actor::IsDead() const {
    return hitPoints <= 0;
}

// all actors can attack functionality
void Actor::Attack(Actor& target, Game& game) {
    auto weapon = GetWeapon();
    if (!weapon) return;
    
        // Calculate the attack and defense points
        int attackerPoints = GetDexterityPoints() + weapon->GetDexterityBonus();
        int defenderPoints = target.GetDexterityPoints() + target.GetArmorPoints();
    
        // Determine if the attack hits
        if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)) {
            attackedHit = true;
    
            // Calculate damage
            int damagePoints = randInt(0, GetStrengthPoints() + weapon->GetDamageAmount() - 1);
            target.SetHitPoints(target.GetHitPoints() - damagePoints);
    
            // Check if the monster is dead
            if (target.IsDead()) {
                target.OnDeath(game);
                finalBlow = true;
            } else {
                finalBlow = false;
            }
        } else {
            // Attack missed
            attackedHit = false;
            finalBlow = false;
        }
    
        attacked = true;
    }
