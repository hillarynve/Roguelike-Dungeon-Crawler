#include "game.h"
#include "utilities.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

Game::Game(int goblinSmellDistance) : map(), player(0, 0, 100, 50, 30, 40) {
    int x, y;
    do {
        x = randInt(MAP_WIDTH);
        y = randInt(MAP_HEIGHT);
    } while (!map.IsSpace(x, y));

    player.SetPosition(x, y);
    initializeMonsters();
}

void Game::activateCheatMode() {
    // update to cheated statistics
    player.SetStrengthPoints(9);
    player.SetHitPoints(50);
}

///////////////////////////////////////////////////////////////////////////
// inititalize game objects & monsters to randomly spawn on the map
///////////////////////////////////////////////////////////////////////////
///
void Game::spawnGameObjects() {
    // add new game objects from vector
    std::vector<GameObjects*> gameObjects = {
        new ShortSword(),
        new LongSword(),
        new Mace()
    };
    // randomize the gameObject's positions within the map
    for (auto& gameObject : gameObjects) {
        int x, y;
        do {
            x = randInt(MAP_WIDTH);
            y = randInt(MAP_HEIGHT);
        } while (!map.IsSpace(x, y));

        gameObject->SetPosition(x, y);
        map.PlaceGameObject(gameObject);
    }
}

void Game::initializeMonsters() {
    // Adding one of each type of monster
    std::vector<Monster*> monsterTypes = {
        new Bogeyman(0, 0, "Bogeyman", 100, 40, 30, 20),
        new Dragon(0, 0, "Dragon", 150, 60, 50, 30),
        new Goblin(0, 0, "Goblin", 80, 20, 40, 50),
        new Snakewoman(0, 0, "Snakewoman", 120, 30, 20, 40)
    };

    // randomize the monster's position on the map
    for (auto& monster : monsterTypes) {
        int x, y;
        do {
            x = randInt(MAP_WIDTH);
            y = randInt(MAP_HEIGHT);
        } while (!map.IsSpace(x, y));

        monster->SetPosition(x, y);
        monsters.push_back(monster);
    }
}

///////////////////////////////////////////////////////////////////////////
// monster movement
///////////////////////////////////////////////////////////////////////////

void Game::moveMonsters() {
    Coordinate playerPos = GetPlayerPosition();
    for (auto monster : monsters) {
        // Check if the monster is a Bogeyman
        if (dynamic_cast<Bogeyman*>(monster)) {
            Bogeyman* bogeyman = dynamic_cast<Bogeyman*>(monster);
            // Calculate direction based on player's position
            char Bdirection = bogeyman->CalculateDirection(playerPos);
            // Move the bogeyman
            bogeyman->Move(Bdirection, map, *this);
        }
        // Check if the monster is a Snakewoman
        else if (dynamic_cast<Snakewoman*>(monster)) {
            Snakewoman* snakewoman = dynamic_cast<Snakewoman*>(monster);
            // Move the snakewoman
            char Sdirection = snakewoman->CalculateDirection(playerPos);
            snakewoman->Move(Sdirection, map, *this);
               }
        // Check if the monster is a Goblin
        else if (dynamic_cast<Goblin*>(monster)) {
            Goblin* goblin = dynamic_cast<Goblin*>(monster);
            // Move the snakewoman
            char Gdirection = goblin->CalculateDirection(playerPos);
            goblin->Move(Gdirection, map, *this);
               }
        
        ///////////////////////////////////////////////////////////////////////////
        // Non-working Goblin recursion implementation
        ///////////////////////////////////////////////////////////////////////////
//         Check of the monster is a Goblin (unused recursion attempt)
//        else if (dynamic_cast<Goblin*>(monster)) {
//            Goblin* goblin = dynamic_cast<Goblin*>(monster);
//            goblin->goblinmove(playerPos.GetX(), playerPos.GetY(), &map, 5); // Assuming maxSteps is defined somewhere accessible
//                }
    }
}

Monster* Game::GetMonsterAt(int x, int y) const{
    for (auto monster : monsters) {
        if (monster->GetPosition().GetX() == x && monster->GetPosition().GetY() == y) {
            return monster;
        }
    }
    return nullptr;
}

void Game::RemoveMonster(Monster* monster) {
    monsters.erase(std::remove(monsters.begin(), monsters.end(), monster), monsters.end());
}

bool Game::isOccupiedByMonster(int x, int y) const {
    for (const auto& monster : monsters) {
        if (monster->GetPosition().GetX() == x && monster->GetPosition().GetY() == y) {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////
// gameObject interaction
///////////////////////////////////////////////////////////////////////////

void Game::pickUpItem() {
    Coordinate playerPos = player.GetPosition();
    for (auto& object : map.GetGameObjects()) {
        if (!object->IsPickedUp() && object->GetPosition().GetX() == playerPos.GetX() && object->GetPosition().GetY() == playerPos.GetY()) {
            player.addItem(std::shared_ptr<GameObjects>(object)); // Wrap object in a shared_ptr
            object->PickUp();
            std::cout << "You picked up a " << object->GetName() << "!" << std::endl;
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
// Play the Game
///////////////////////////////////////////////////////////////////////////

void Game::play() {
    spawnGameObjects();
    
    char input;
    // running loop that takes in keyboard inputs and outputs gameplay mechanics
    do {
        // draw the map if inventory is not selected
        if (!showInventory) {
            map.Draw(player, monsters);
        }

        input = getCharacter();
        
        // quit game with q
        if (input == 'q') {
            break;
        }
        
        // activate cheats
        if (input == 'c') {
            activateCheatMode();
            
        // display inventory
        } else if (input == 'i') {
            if (showInventory) {
                showInventory = false;
            } else {
                showInventory = true;
                player.displayInventory();
                continue; // Skip the rest of the loop to prevent map drawing
            }
        } else {
            showInventory = false; // Reset the flag when other inputs are given
            player.Move(input, map, *this);  // Pass the game instance to the player
            for (auto& monster : monsters) {
                monster->CheckAndAttackPlayer(player, *this);
            }
            moveMonsters();
        }
    } while (true);

    // Clean up dynamically allocated monsters
    for (auto monster : monsters) {
        delete monster;
    }
}

