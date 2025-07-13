#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H
#include "ChessPiece.h"
using namespace std;

/*
 * Concrete implementation of knight chess piece
 */

class Knight : public ChessPiece {
public:
    Knight(int color, int column, int row) : ChessPiece(color, column, row) {}
    bool isValidMove(int destinationCol, int destinationRow, const vector<vector<ChessPiece*>>& board) override;
    ChessPiece * clone() const override;
};

#endif //CHESS_KNIGHT_H
