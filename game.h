#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "player.h"
#include "map.h"
#include "monsters.h"
#include <vector>
#include <algorithm>

class Game {
public:
    Game(int goblinSmellDistance);
    void play();
    // player related
    Coordinate GetPlayerPosition() const {return player.GetPosition(); }
    void activateCheatMode();
    
    // monster related
    bool isOccupiedByMonster(int x, int y) const;
    Monster* GetMonsterAt(int x, int y) const;
    void RemoveMonster(Monster* monster);
   
    

private:
    Player player;
    Map map;
    
    // monsters
    std::vector<Monster*> monsters; // Store pointers to monsters
    void initializeMonsters(); // Function to initialize monsters
    void moveMonsters(); // Function to move monsters
    
    // inventory
    std::vector<GameObjects*> weapons;
    bool showInventory = false;
    
    // gameObjects
    void spawnGameObjects();
    void pickUpItem();

};

#endif // GAME_INCLUDED
