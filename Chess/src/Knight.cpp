#include "Knight.h"
#include <cmath>

/*
 * Knight class implementation
 */

bool Knight::isValidMove(int destinationCol, int destinationRow) {
    int deltaColumn = abs(this->column - destinationCol);
    int deltaRow = abs(this->row - destinationRow);

    // L shape movement
    if ((deltaColumn == 2 && deltaRow == 1) || (deltaColumn == 1 && deltaRow == 2)) {
        return true;
    }

    return false;
}