#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H
#include "ChessPiece.h"

class Bishop : public ChessPiece {
    Bishop(int color, int column, int row) : ChessPiece(color, column, row) {}
    bool isValidMove(int destinationCol, int destinationRow) override;
};

#endif //CHESS_BISHOP_H
