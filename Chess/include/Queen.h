#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H
#include "ChessPiece.h"

/*
 * Concrete implementation of queen chess piece
 */

class Queen : public ChessPiece {
public:
    Queen(int color, int column, int row) : ChessPiece(color, column, row) {}
    bool isValidMove(int destinationCol, int destinationRow) override;
    ChessPiece * clone() const override;
};

#endif //CHESS_QUEEN_H
