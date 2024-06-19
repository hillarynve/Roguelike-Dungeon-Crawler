#include "map.h"
#include "player.h"
#include "constants.h"
#include "utilities.h"
#include "coordinate.h"
#include <iostream>
#include <vector>
#include "monsters.h"

using namespace std;

Map::Map() {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            grid[i][j] = '#';
        }
    }
    CreateRoomsAndCorridors();
}

///////////////////////////////////////////////////////////////////////////
// generate random rooms & corridors
///////////////////////////////////////////////////////////////////////////

void Map::CreateRoomsAndCorridors() {
    int numRooms = randInt(4, 6);
    vector<Room> rooms;

    // loop until enough rooms are generated
    for (int i = 0; i < numRooms; ++i) {
        bool placed = false;
        for (int attempt = 0; attempt < 100 && !placed; ++attempt) {
            // randomize the room's size & top-left coordiante
            int roomWidth = randInt(8, MAP_WIDTH / 2);
            int roomHeight = randInt(3, MAP_HEIGHT / 2);
            int roomX = randInt(1, MAP_WIDTH - roomWidth - 1);
            int roomY = randInt(1, MAP_HEIGHT - roomHeight - 1);

            Room newRoom(roomX, roomY, roomX + roomWidth - 1, roomY + roomHeight - 1);
            
            // avoid overlapping rooms
            bool overlaps = false;
            for (const Room& room : rooms) {
                if (newRoom.overlapsWith(room)) {
                    overlaps = true;
                    break;
                }
            }

            if (!overlaps) {
                for (int y = roomY; y <= roomY + roomHeight - 1; ++y) {
                    for (int x = roomX; x <= roomX + roomWidth - 1; ++x) {
                        grid[y][x] = ' ';
                    }
                }
                rooms.push_back(newRoom);
                placed = true;
            }
        }
    }
    
    // generate corridors by comeparing two adjacent room's midpoints. make a horizontal or vertical path connecting them.
    for (size_t i = 1; i < rooms.size(); ++i) {
        auto [prevX, prevY] = make_pair((rooms[i - 1].getX1() + rooms[i - 1].getX2()) / 2, (rooms[i - 1].getY1() + rooms[i - 1].getY2()) / 2);
        auto [currX, currY] = make_pair((rooms[i].getX1() + rooms[i].getX2()) / 2, (rooms[i].getY1() + rooms[i].getY2()) / 2);

        if (prevX < currX) {
            for (int x = prevX; x <= currX; ++x) {
                grid[prevY][x] = ' ';
            }
        } else {
            for (int x = currX; x <= prevX; ++x) {
                grid[prevY][x] = ' ';
            }
        }

        if (prevY < currY) {
            for (int y = prevY; y <= currY; ++y) {
                grid[y][currX] = ' ';
            }
        } else {
            for (int y = currY; y <= prevY; ++y) {
                grid[y][currX] = ' ';
            }
        }
    }
}

bool Map::IsSpace(int x, int y) const {
    return grid[y][x] == ' ';
}

///////////////////////////////////////////////////////////////////////////
// Display the map
///////////////////////////////////////////////////////////////////////////

void Map::Draw(const Player& player, const std::vector<Monster*>& monsters) const {
    clearScreen(); // clear the screen
    Coordinate playerPos = player.GetPosition();
    
    // loop through the 2D grid map
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            bool isMonster = false;
            bool isItem = false;
            
            // display monsters
            for (const auto& monster : monsters) {
                if (monster->GetPosition().GetX() == j && monster->GetPosition().GetY() == i) {
                    cout << monster->GetSymbol(); // draw monster
                    isMonster = true;
                    break;
                }
            }
            
            // display gameObjects
            for (const auto& object : gameObjects) {
                if (!object->IsPickedUp() && object->GetPosition().GetX() == j && object->GetPosition().GetY() == i) {
                    cout << object->GetSymbol(); // draw game object
                    isItem = true;
                    break;
                }
            }
            
            // display the player & the rest of the stored grid map ('#' or ' ')
            if (!isMonster) {
                if (playerPos.GetX() == j && playerPos.GetY() == i) {
                    cout << '@'; // draw player
                } else {
                    if (!isItem)
                        cout << grid[i][j]; // draw map
                }
            }
        }
        cout << endl;
    }
    // display stats
    cout << "Level: 0, Hit points: " << player.GetHitPoints() << ", Armor: " << player.GetArmorPoints() << ", Strength: " << player.GetStrengthPoints() << ", Dexterity: " << player.GetDexterityPoints() << endl << endl;
    
    // Display weapon action message
        if (player.HasAttacked()) {
            auto weapon = player.GetWeapon();
            if (weapon) {
                if (player.IsFinalBlow()) {
                    cout << "Player " << weapon->GetAction() << "the " << player.GetCurrentTargetMonster()->GetName() << " dealing a final blow!]." << endl;
                } else {
                    if (player.DidAttackHit()) {
                        cout << "Player " << weapon->GetAction() << "the " << player.GetCurrentTargetMonster()->GetName() << " and hits." << endl;
                    } else {
                        cout << "Player " << weapon->GetAction() << "the " << player.GetCurrentTargetMonster()->GetName() << " and misses." << endl;
                    }
                }
            }
        }
    
    // Display monster's attack message
      for (const auto& monster : monsters) {
          if (monster->HasAttacked()) {
              auto weapon = monster->GetWeapon();
              if (weapon) {
                  if (monster->IsFinalBlow()) {
                      cout << monster->GetName() << " " << weapon->GetAction() << "the Player dealing a final blow." << endl;
                  } else {
                      if (monster->DidAttackHit()) {
                          cout << monster->GetName() << " " << weapon->GetAction() << "the Player and hits." << endl;
                      } else {
                          cout << monster->GetName() << " " << weapon->GetAction() << "the Player and misses." << endl;
                      }
                  }
              }
          }
      }
  }

///////////////////////////////////////////////////////////////////////////
// Game Object integration
///////////////////////////////////////////////////////////////////////////

void Map::PlaceGameObject(GameObjects* object) {
    int x, y;
    do {
        x = rand() % MAP_WIDTH;
        y = rand() % MAP_HEIGHT;
    } while (!IsPositionValid(x, y));

    object->SetPosition(x, y);
    gameObjects.push_back(object);
}

bool Map::IsPositionValid(int x, int y) const {
    return grid[y][x] != '#';
}

const std::vector<GameObjects*>& Map::GetGameObjects() const {
    return gameObjects;
}


