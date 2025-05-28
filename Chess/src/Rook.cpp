#include "Rook.h"

/*
 * Rook class implementation
 */

bool Rook::isValidMove(int destinationCol, int destinationRow) {
    // Rook piece is able to move only in straight lines
    return this->column == destinationCol || this->row == destinationRow;
}

ChessPiece *Rook::clone() const {
    return new Rook(*this);
}