#include "Pawn.h"
#include "Colors.h"
#include <cmath>

/*
 * Pawn class implementation
 */

bool Pawn::isValidMove(int destinationCol, int destinationRow) {
    int startRow = (this->color == Colors::White) ? 6 : 1;
    int deltaColumn = abs(this->column - destinationCol);
    int deltaRow = abs(this->row - destinationRow);

    // standard one-step forward move
    if (deltaColumn == 0 && deltaRow == 1) {
        return true;
    }

    // initial two-step forward move
    if (deltaColumn == 0 && this->row == startRow && deltaRow == 2) {
        return true;
    }

    // diagonal capture
    if (deltaRow == deltaColumn && deltaRow == 1) {
        ChessPiece* capturedPiece = board[destinationRow][destinationCol];
        if (capturedPiece != nullptr) {
            bool isEnemy = capturedPiece->getColor() != this->color;
            return isEnemy;
        }
    }

    return false;
}

ChessPiece *Pawn::clone() const {
    return new Pawn(*this);
}