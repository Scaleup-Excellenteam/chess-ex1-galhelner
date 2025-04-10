#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H
#include "ChessPiece.h"

/*
 * Concrete implementation of bishop chess piece
 */

class Bishop : public ChessPiece {
public:
    Bishop(int color, int column, int row) : ChessPiece(color, column, row) {}
    bool isValidMove(int destinationCol, int destinationRow) override;
};

#endif //CHESS_BISHOP_H
