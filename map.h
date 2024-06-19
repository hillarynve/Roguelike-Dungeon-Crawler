#ifndef MAP_H
#define MAP_H

#include "constants.h"
#include "coordinate.h"
#include <vector>
#include "gameobjects.h"

class Monster;
class Player; // Forward declaration of Player

class Room {
public:
    Room(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
    int getX1() const { return x1; }
    int getY1() const { return y1; }
    int getX2() const { return x2; }
    int getY2() const { return y2; }
    bool overlapsWith(const Room& other) const {
        // Check if this room overlaps with another room, considering a 1-tile buffer
        return !(x2 + 1 < other.x1 || x1 > other.x2 + 1 || y2 + 1 < other.y1 || y1 > other.y2 + 1);
    }
private:
    int x1, y1, x2, y2;
};

class Map {
public:
    Map();
    void Draw(const Player& player, const std::vector<Monster*>& monsters) const;
    bool IsSpace(int x, int y) const;
    
    // items
    void PlaceGameObject(GameObjects* object);
    bool IsPositionValid(int x, int y) const;
    const std::vector<GameObjects*>& GetGameObjects() const;
    
private:
    char grid[MAP_HEIGHT][MAP_WIDTH];
    void CreateRoomsAndCorridors();
    
    std::vector<GameObjects*> gameObjects;
};

#endif // MAP_H
