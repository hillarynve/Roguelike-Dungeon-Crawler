//
//  gameobjects.hpp
//
//
//  Created by Hillary Nguyen on 5/23/24.
//
#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "coordinate.h"
#include <string>

class GameObjects {
public:
    GameObjects(const std::string& name, char symbol);  // Add this constructor
    virtual ~GameObjects() = default;

    Coordinate GetPosition() const;
    void SetPosition(int x, int y);
    std::string GetName() const;
    char GetSymbol() const;
    
    // iteract with gameObject
    bool IsPickedUp() const;
    void PickUp();

private:
    Coordinate position;
    std::string name;
    char symbol;
    bool pickedUp;
};

#endif // GAMEOBJECTS_H
