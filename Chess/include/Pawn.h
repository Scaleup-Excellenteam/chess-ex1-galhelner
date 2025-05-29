#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H
#include "ChessPiece.h"
#include <vector>
using namespace std;

/*
 * Concrete implementation of pawn chess piece
 */

class Pawn : public ChessPiece {
public:
    Pawn(int color, int column, int row) : ChessPiece(color, column, row) {}
    bool isValidMove(int destinationCol, int destinationRow, const vector<vector<ChessPiece*>>& board) override;
    ChessPiece * clone() const override;
};
#endif //CHESS_PAWN_H
