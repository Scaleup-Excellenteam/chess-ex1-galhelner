#include "Bishop.h"

bool Bishop::isValidMove(int destinationCol, int destinationRow) {
    // Bishop piece is able to move only diagonally
    return this->column != destinationCol && this->row != destinationRow;
}