#include "gameobjects.h"

GameObjects::GameObjects(const std::string& name, char symbol)
    : name(name), symbol(symbol), pickedUp(false) {}



Coordinate GameObjects::GetPosition() const {
    return position;
}

void GameObjects::SetPosition(int x, int y) {
    position.SetX(x);
    position.SetY(y);
}

std::string GameObjects::GetName() const {
    return name;
}

char GameObjects::GetSymbol() const {
    return symbol;
}

bool GameObjects::IsPickedUp() const {
    return pickedUp;
}

void GameObjects::PickUp() {
    pickedUp = true;
}
