#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H
#include "ChessPiece.h"
#include <vector>
using namespace std;

/*
 * Concrete implementation of pawn chess piece
 */

class Pawn : public ChessPiece {
private:
    vector<vector<ChessPiece*>>& board;
public:
    Pawn(int color, int column, int row, vector<vector<ChessPiece*>>& board) : ChessPiece(color, column, row), board(board) {}
    bool isValidMove(int destinationCol, int destinationRow) override;
    ChessPiece * clone() const override;
};
#endif //CHESS_PAWN_H
