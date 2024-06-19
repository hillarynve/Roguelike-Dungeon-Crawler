#include "monsters.h"
#include "map.h"
#include "game.h"
#include <iostream>
#include "utilities.h"
#include "player.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////
// Monster Base Class implementation, Derived from Actor class
///////////////////////////////////////////////////////////////////////////
Monster::Monster(int x, int y, const std::string& name, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints)
    : Actor(x, y, hitPoints, armorPoints, strengthPoints, dexterityPoints), attacked(false), finalBlow(false) {}


void Monster::OnDeath(Game& game) {
    game.RemoveMonster(this);
}

void Monster::Attack(Player& player, Game& game) {
    Actor::Attack(player, game);
    
    // Set the attacked and final blow flags 
        if (player.IsDead()) {
            finalBlow = true;
        }
        attacked = true;
}

void Monster::CheckAndAttackPlayer(Player& player, Game& game) {
    Coordinate playerPos = player.GetPosition();
    Coordinate monsterPos = GetPosition();

    // Check if the player is adjacent (up, down, left, right)
    if ((playerPos.GetX() == monsterPos.GetX() && abs(playerPos.GetY() - monsterPos.GetY()) == 1) ||
        (playerPos.GetY() == monsterPos.GetY() && abs(playerPos.GetX() - monsterPos.GetX()) == 1)) {
        Attack(player, const_cast<Game&>(game));
    }
}

///////////////////////////////////////////////////////////////////////////
// Bogeyman implementation
///////////////////////////////////////////////////////////////////////////
Bogeyman::Bogeyman(int x, int y, const std::string& name, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints)
    : Monster(x, y, "Bogeyman", randInt(5, 10), 2, randInt(2, 3), randInt(2, 3)) {
        SetName("Bogeyman");
        SetWeapon(std::make_shared<Mace>());
    }

void Bogeyman::Move(char direction, const Map& map, const Game& game) {
    Coordinate playerPos = game.GetPlayerPosition();
    Coordinate bogeymanPos = GetPosition();

    // Calculate the Manhattan distance between the player and the bogeyman
    int distance = abs(playerPos.GetX() - bogeymanPos.GetX()) + abs(playerPos.GetY() - bogeymanPos.GetY());

    // Check if the bogeyman can smell the player within 5 steps
    if (distance <= 5) {
        // Calculate the maximum steps the bogeyman can take
        int maxSteps = std::min(5, distance);

        // Move one step closer to the player's row or column if possible
        int newX = bogeymanPos.GetX();
        int newY = bogeymanPos.GetY();
        if (direction == 'u') {
            newY -= 1;
        } else if (direction == 'd') {
            newY += 1;
        } else if (direction == 'l') {
            newX -= 1;
        } else if (direction == 'r') {
            newX += 1;
        }

        // Check if the new position is valid and not occupied by a wall or another monster
        if (maxSteps > 0 && map.IsSpace(newX, newY) && !game.isOccupiedByMonster(newX, newY) && (playerPos.GetX() != newX || playerPos.GetY() != newY)) {
            // Move one step closer to the player's row or column
            SetPosition(newX, newY);
        }
    }
}


char Bogeyman::CalculateDirection(const Coordinate& playerPos) const {
    Coordinate bogeymanPos = GetPosition();

    // Calculate the difference in positions
    int deltaX = playerPos.GetX() - bogeymanPos.GetX();
    int deltaY = playerPos.GetY() - bogeymanPos.GetY();

    // Determine the direction based on the difference
    if (abs(deltaX) > abs(deltaY)) {
        return (deltaX > 0) ? 'r' : 'l'; // 'r' for right, 'l' for left
    } else {
        return (deltaY > 0) ? 'd' : 'u'; // 'd' for down, 'u' for up
    }
}

///////////////////////////////////////////////////////////////////////////
// Dragon implementation
///////////////////////////////////////////////////////////////////////////

// Dragon implementation
Dragon::Dragon(int x, int y, const std::string& name, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints)
: Monster(x, y, "Dragon", randInt(20, 25), 4, 4, 4) {
    SetName("Dragon");
    SetWeapon(std::make_shared<LongSword>());
    }

void Dragon::Move(char direction, const Map& map, const Game& game) {
    // dragon does not move
}

void Dragon::RegenerateHealth() {
    // Check if hit points are less than the maximum
    if (hitPoints < GetHitPoints()) {
        // Regenerate hit points with a probability of 1 in 10
        if (trueWithProbability(0.1)) {
            ++hitPoints;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
// Goblin implementation (non-recursive), see recursion attempt & logic below.
///////////////////////////////////////////////////////////////////////////
Goblin::Goblin(int x, int y, const std::string& name, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints)
: Monster(x, y, "Goblin", randInt(15, 20), 1, 3, 1), posRow_(x), posCol_(y) {
    SetName("Goblin");
    SetWeapon(std::make_shared<ShortSword>());
    }

void Goblin::Move(char direction, const Map& map, const Game& game) {
    Coordinate playerPos = game.GetPlayerPosition();
    Coordinate goblinPos = GetPosition();

    // Calculate the Manhattan distance between the player and the goblin
    int distance = abs(playerPos.GetX() - goblinPos.GetX()) + abs(playerPos.GetY() - goblinPos.GetY());

    // Check if the goblin can smell the player within 5 steps
    if (distance <= 15) {
        // Calculate the maximum steps the snakeWoman can take
        int maxSteps = std::min(15, distance);

        // Move one step closer to the player's row or column if possible
        int newX = goblinPos.GetX();
        int newY = goblinPos.GetY();
        if (direction == 'u') {
            newY -= 1;
        } else if (direction == 'd') {
            newY += 1;
        } else if (direction == 'l') {
            newX -= 1;
        } else if (direction == 'r') {
            newX += 1;
        }

        // Check if the new position is valid and not occupied by a wall or another monster or the player
        if (maxSteps > 0 && map.IsSpace(newX, newY) && !game.isOccupiedByMonster(newX, newY) && (playerPos.GetX() != newX || playerPos.GetY() != newY)) {
            // Move one step closer to the player's row or column
            SetPosition(newX, newY);
        }
    }
}

char Goblin::CalculateDirection(const Coordinate& playerPos) const {
    Coordinate goblinPos = GetPosition();

    // Calculate the difference in positions
    int deltaX = playerPos.GetX() - goblinPos.GetX();
    int deltaY = playerPos.GetY() - goblinPos.GetY();

    // Determine the direction based on the difference
    if (abs(deltaX) > abs(deltaY)) {
        return (deltaX > 0) ? 'r' : 'l'; // 'r' for right, 'l' for left
    } else {
        return (deltaY > 0) ? 'd' : 'u'; // 'd' for down, 'u' for up
    }
}
///////////////////////////////////////////////////////////////////////////
// Snakewoman implementation
///////////////////////////////////////////////////////////////////////////
///
Snakewoman::Snakewoman(int x, int y,  const std::string& name, int hitPoints, int armorPoints, int strengthPoints, int dexterityPoints)
: Monster(x, y, "Snakewoman", randInt(3, 6), 3, 2, 3) {
    SetName("Snakewoman");
    SetWeapon(std::make_shared<MagicFangsOfSleep>());
    }

void Snakewoman::Move(char direction, const Map& map, const Game& game) {
    Coordinate playerPos = game.GetPlayerPosition();
    Coordinate snakeWomanPos = GetPosition();

    // Calculate the Manhattan distance between the player and the snakeWoman
    int distance = abs(playerPos.GetX() - snakeWomanPos.GetX()) + abs(playerPos.GetY() - snakeWomanPos.GetY());

    // Check if the snakeWoman can smell the player within 5 steps
    if (distance <= 3) {
        // Calculate the maximum steps the snakeWoman can take
        int maxSteps = std::min(3, distance);

        // Move one step closer to the player's row or column if possible
        int newX = snakeWomanPos.GetX();
        int newY = snakeWomanPos.GetY();
        if (direction == 'u') {
            newY -= 1;
        } else if (direction == 'd') {
            newY += 1;
        } else if (direction == 'l') {
            newX -= 1;
        } else if (direction == 'r') {
            newX += 1;
        }

        // Check if the new position is valid and not occupied by a wall or another monster or the player
        if (maxSteps > 0 && map.IsSpace(newX, newY) && !game.isOccupiedByMonster(newX, newY) && (playerPos.GetX() != newX || playerPos.GetY() != newY)) {
            // Move one step closer to the player's row or column
            SetPosition(newX, newY);
        }
    }
}

char Snakewoman::CalculateDirection(const Coordinate& playerPos) const {
    Coordinate snakeWomanPos = GetPosition();

    // Calculate the difference in positions
    int deltaX = playerPos.GetX() - snakeWomanPos.GetX();
    int deltaY = playerPos.GetY() - snakeWomanPos.GetY();

    // Determine the direction based on the difference
    if (abs(deltaX) > abs(deltaY)) {
        return (deltaX > 0) ? 'r' : 'l'; // 'r' for right, 'l' for left
    } else {
        return (deltaY > 0) ? 'd' : 'u'; // 'd' for down, 'u' for up
    }
}










///////////////////////////////////////////////////////////////////////////
// Attempted Goblin Recursion Implementation (doesn't work, notes in doc)
///////////////////////////////////////////////////////////////////////////
//
//
//bool Goblin::reachesPlayer(int g_r, int g_c, int p_r, int p_c, std::vector<std::pair<int, int>>& visited, std::vector<std::pair<int, int>>& bestPath, int maxSteps, Map* m) {
////    std::cout << "Checking reachesPlayer for goblin at (" << g_r << ", " << g_c << ") and player at (" << p_r << ", " << p_c << ")" << std::endl;
//
//    if (visited.size() >= maxSteps) {
////        std::cout << "Max steps reached." << std::endl;
//        return false;
//    }
//
//    if (abs(g_r - p_r) + abs(g_c - p_c) == 1) {
////        std::cout << "Goblin adjacent to player." << std::endl;
//
//        visited.push_back(std::make_pair(g_r, g_c));
//        if (bestPath.empty() || visited.size() < bestPath.size()) {
//            bestPath = visited;
//        }
//        return true;
//    }
//
//    for (std::vector<std::pair<int, int>>::iterator i = visited.begin(); i != visited.end(); i++) {
//        if (i->first == g_r && i->second == g_c) {
////            std::cout << "Already visited goblin's position." << std::endl;
//            return false;
//        }
//    }
//
//    visited.push_back(std::make_pair(g_r, g_c));
//
//    bool pathFound = false;
//    
//    // check right
//    if (m->IsSpace(g_r, g_c + 1) && p_c > g_c) {
////        std::cout << "Checking right." << std::endl;
//        
//        if (reachesPlayer(g_r, g_c + 1, p_r, p_c, visited, bestPath, maxSteps, m)) {
//            pathFound = true;
//            if (bestPath.empty() || visited.size() < bestPath.size()) {
//                bestPath = visited;
//            }
//        }
//    }
//
//    // check left
//    if (m->IsSpace(g_r, g_c-1) && p_c > g_c ) { // if the player is on the left of the goblin and the goblin is able to move to the left one step
////        std::cout << "Checking left." << std::endl;
//        if (reachesPlayer(g_r, g_c-1, p_r, p_c, visited, bestPath, maxSteps, m)) {
//            pathFound = true;
//            if (bestPath.empty() || visited.size() < bestPath.size()) {
//                bestPath = visited; // if this newly traversed path is better than the best path so far (meaning that it takes less steps to get to the player), replace best path with the current path
//            }
//        }
//    }
//    // check up
//    if (m->IsSpace(g_r-1, g_c) && p_c > g_c ) { // if the player is on above the goblin and the goblin is able to move to one step up
////        std::cout << "Checking up." << std::endl;
//        if (reachesPlayer(g_r-1, g_c, p_r, p_c, visited, bestPath, maxSteps, m)) {
//            pathFound = true;
//            if (bestPath.empty() || visited.size() < bestPath.size()) {
//                bestPath = visited; // if this newly traversed path is better than the best path so far (meaning that it takes less steps to get to the player), replace best path with the current path
//            }
//        }
//    }
//    
//    // check down
//    if (m->IsSpace(g_r+1, g_c) && p_c > g_c ) { // if the player is on below the goblin and the goblin is able to move to one step down
////        std::cout << "Checking down." << std::endl;
//        if (reachesPlayer(g_r+1, g_c, p_r, p_c, visited, bestPath, maxSteps, m)) {
//            pathFound = true;
//            if (bestPath.empty() || visited.size() < bestPath.size()) {
//                bestPath = visited; // if this newly traversed path is better than the best path so far (meaning that it takes less steps to get to the player), replace best path with the current path
//            }
//        }
//    }
//
//    visited.pop_back();
//    std::cout << "failed here?" << std::endl;
//    return pathFound;
//}
//
//// commented out debugging methods
//void Goblin::goblinmove(int r, int c, Map* m, int maxSteps) {
////    std::cout << "Goblin moving towards player at (" << r << ", " << c << ")" << std::endl;
//    std::vector<std::pair<int, int>> visited;
//    std::vector<std::pair<int, int>> bestPath;
//
//    Coordinate goblinPos = GetPosition();
//    int goblinX = goblinPos.GetX();
//    int goblinY = goblinPos.GetY();
//    if (reachesPlayer(goblinX, goblinY, r, c, visited, bestPath, maxSteps, m)) {
//        if (bestPath.size() > 1) {
//            // Get the next position from the bestPath
//            int next_r = bestPath[1].first;
//            int next_c = bestPath[1].second;
////            std::cout << "Next position for goblin: (" << next_r << ", " << next_c << ")" << std::endl;
//
//            // Check if the new position is valid
//            if (m->IsSpace(next_r, next_c)) {
////                          std::cout << "Moving goblin to (" << next_r << ", " << next_c << ")" << std::endl;
//                          SetPosition(next_r, next_c);
//                          goblinX = next_r; //row
//                          goblinY = next_c; //col
//                      } else {
////                          std::cout << "Next position is not a valid space." << std::endl;
//                      }
//                  } else {
////                      std::cout << "Best path not found." << std::endl;
//                  }
//              }
//    else {
////        std::cout << "Goblin unable to reach player." << std::endl;
//        }
//    }
