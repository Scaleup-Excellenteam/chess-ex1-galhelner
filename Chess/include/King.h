#ifndef CHESS_KING_H
#define CHESS_KING_H
#include "ChessPiece.h"

/*
 * Concrete implementation of king chess piece
 */

class King : public ChessPiece {
public:
    King(int color, int column, int row) : ChessPiece(color, column, row) {}
    bool isValidMove(int destinationCol, int destinationRow) override;
};

#endif //CHESS_KING_H
