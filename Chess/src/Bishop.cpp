#include "Bishop.h"
#include <cmath>

/*
 * Bishop class implementation
 */

bool Bishop::isValidMove(int destinationCol, int destinationRow, const vector<vector<ChessPiece*>>& board) {
    // Bishop piece is able to move only diagonally
    int deltaColumn = abs(this->column - destinationCol);
    int deltaRow = abs(this->row - destinationRow);
    return deltaRow == deltaColumn;
}

ChessPiece *Bishop::clone() const {
    return new Bishop(color, column, row);
}