#include "Rock.h"

bool Rock::isValidMove(int destinationCol, int destinationRow) {
    // Rock piece is able to move only in straight lines
    return this->column == destinationCol || this->row == destinationRow;
}
