#include "Rook.h"

bool Rook::isValidMove(int destinationCol, int destinationRow) {
    // Rook piece is able to move only in straight lines
    return this->column == destinationCol || this->row == destinationRow;
}
