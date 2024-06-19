// used to keep track of positions
#ifndef COORDINATE_H
#define COORDINATE_H

class Coordinate {
public:
    Coordinate(int x = 0, int y = 0) : x(x), y(y) {}
    int GetX() const { return x; }
    int GetY() const { return y; }
    void SetX(int x) { this->x = x; }
    void SetY(int y) { this->y = y; }
    
    // Equality comparison operator
        bool operator==(const Coordinate& other) const {
            return x == other.x && y == other.y;
        }

private:
    int x;
    int y;
};

#endif // COORDINATE_H
