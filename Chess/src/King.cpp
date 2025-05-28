#include "King.h"
#include <cmath>

/*
 * King class implementation
 */

bool King::isValidMove(int destinationCol, int destinationRow) {
    // King piece is able to move only single step sideways or diagonally
    int deltaColumn = abs(this->column - destinationCol);
    int deltaRow = abs(this->row - destinationRow);
    return deltaColumn <= 1 && deltaRow <= 1;
}

ChessPiece *King::clone() const {
    return new King(*this);
}