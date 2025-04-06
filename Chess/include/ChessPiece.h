#ifndef CHESS_CHESSPIECE_H
#define CHESS_CHESSPIECE_H

class ChessPiece {
public:
    ChessPiece(int color, int column, int row);
    virtual bool isValidMove(int destinationCol, int destinationRow) = 0;

protected:
    int color;
    int column;
    int row;
};

#endif //CHESS_CHESSPIECE_H
