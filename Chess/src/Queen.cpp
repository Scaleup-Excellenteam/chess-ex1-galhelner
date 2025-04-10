#include "Queen.h"
#include <cmath>

bool Queen::isValidMove(int destinationCol, int destinationRow) {
    // Queen piece is able to move in any direction
    if (this->column == destinationCol || this->row == destinationRow) {
        return true;
    }

    int deltaColumn = abs(this->column - destinationCol);
    int deltaRow = abs(this->row - destinationRow);
    return deltaRow == deltaColumn;
}