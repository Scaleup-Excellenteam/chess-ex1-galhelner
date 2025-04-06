#ifndef CHESS_ROCK_H
#define CHESS_ROCK_H
#include "ChessPiece.h"

class Rock : public ChessPiece {
public:
    Rock(int color, int column, int row): ChessPiece(color, column, row) {}
    bool isValidMove(int destinationCol, int destinationRow) override;
};

#endif //CHESS_ROCK_H
