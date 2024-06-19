#include "player.h"
#include "utilities.h"
#include "game.h"
#include <cstdlib> 

Player::Player(int x, int y, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints)
    : Actor(x, y, 20, 2, 2, 2), level(0), attacked(false), finalBlow(false) {
        // Player starts with a Short Sword
        SetWeapon(std::make_shared<ShortSword>());
        inventory.push_back(weapon);
    }

///////////////////////////////////////////////////////////////////////////
// player movement
///////////////////////////////////////////////////////////////////////////
void Player::Move(char direction, const Map& map, const Game& game) {
    int newX = position.GetX();
    int newY = position.GetY();

    // ensure keyboard inputs cause correct directional change
    switch (direction) {
        case ARROW_UP:
            newY -= 1;
            break;
        case ARROW_DOWN:
            newY += 1;
            break;
        case ARROW_LEFT:
            newX -= 1;
            break;
        case ARROW_RIGHT:
            newX += 1;
            break;
        default:
            return; // Ignore any other key
    }
    
    attacked = false;
    
    // check to make sure player does not move into invalid space
    if (map.IsSpace(newX, newY) && !game.isOccupiedByMonster(newX, newY)) {
            SetPosition(newX, newY);
        } else {
            Monster* monster = game.GetMonsterAt(newX, newY);
                    if (monster != nullptr) {
                        /*Attack(*monster, const_cast<Game&>(game));*/ // Pass the game instance to the Attack method
                        Attack(*monster, const_cast<Game&>(game));
            }
        }

}

// regenerate health
void Player::regenerateHealth() {
    if (trueWithProbability(0.1)) {
        if (hitPoints < 20) {
            ++hitPoints;
        }
    }
}

int Player::GetLevel() const {
    return level;
}

void Player::SetLevel(int level) {
    this->level = level;
}

///////////////////////////////////////////////////////////////////////////
// Player attack monsters functionality
///////////////////////////////////////////////////////////////////////////
void Player::Attack(Monster& monster, Game& game) {
    currentTargetMonster = &monster;
    Actor::Attack(monster, game);
    attacked = true;
    attackedHit = monster.GetHitPoints() <= 0 || DidAttackHit();
    finalBlow = monster.IsDead();
}
// Implement the SetTargetMonster, ClearTargetMonster, and IsFinalBlow methods
void Player::SetTargetMonster(Monster* monster) {
    currentTargetMonster = monster;
}

void Player::ClearTargetMonster() {
    currentTargetMonster = nullptr;
}

void Player::OnDeath(Game& game) {
    if (GetHitPoints() <= 0) {
        std::cout << "Game Over! You have died." << std::endl;
        // End the game
        // For example, you can call a function to quit the game
        exit(0);
    }
}

///////////////////////////////////////////////////////////////////////////
// Player inventory system
///////////////////////////////////////////////////////////////////////////
///
void Player::addItem(std::shared_ptr<GameObjects> item) {
    if (inventory.size() <= 25) { // Only add if inventory has 25 or fewer items
        inventory.push_back(item);
    }
}

void Player::displayInventory() const {
    std::cout << std::endl << "Inventory:" << std::endl;
    char label = 'a';
    for (const auto& item : inventory) {
        std::cout << " " << label++ << ". " << item->GetName() << std::endl;
    }
}
