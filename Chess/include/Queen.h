#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H
#include "ChessPiece.h"

class Queen : public ChessPiece {
    Queen(int color, int column, int row) : ChessPiece(color, column, row) {}
    bool isValidMove(int destinationCol, int destinationRow) override;
};

#endif //CHESS_QUEEN_H
