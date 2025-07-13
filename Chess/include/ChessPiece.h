#ifndef CHESS_CHESSPIECE_H
#define CHESS_CHESSPIECE_H
#include "iostream"
#include "vector"
using namespace std;

/*
 * Abstract class of chess piece object
 */

class ChessPiece {
public:
    ChessPiece(int color, int column, int row);
    virtual bool isValidMove(int destinationCol, int destinationRow, const vector<vector<ChessPiece*>>& board) = 0;
    virtual ChessPiece* clone() const = 0;
    virtual ~ChessPiece() = default;
    virtual int getColor();
    void setColumn(int newColumn) { this->column = newColumn; }
    void setRow(int newRow) { this->row = newRow; }

protected:
    int color;
    int column;
    int row;
};

#endif //CHESS_CHESSPIECE_H
