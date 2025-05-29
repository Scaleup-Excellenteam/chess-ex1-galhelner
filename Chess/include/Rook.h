#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H
#include "ChessPiece.h"

/*
 * Concrete implementation of rook chess piece
 */

class Rook : public ChessPiece {
public:
    Rook(int color, int column, int row): ChessPiece(color, column, row) {}
    bool isValidMove(int destinationCol, int destinationRow, const vector<vector<ChessPiece*>>& board) override;
    ChessPiece * clone() const override;
};

#endif //CHESS_ROOK_H
