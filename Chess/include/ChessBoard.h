#ifndef CHESS_CHESSBOARD_H
#define CHESS_CHESSBOARD_H
#include <string>
#include <vector>
#include "ChessPiece.h"
using namespace std;

class ChessBoard {
private:
    static ChessBoard* instance;
    ChessBoard();
    vector<vector<ChessPiece*>> board;
    void addPiece(char symbol, int column, int row);
    vector<vector<ChessPiece*>>& getBoard();

public:
    ChessBoard(const ChessBoard&) = delete;
    ChessBoard& operator= (const ChessBoard&) = delete;
    static ChessBoard* getInstance(const string& boardString);
};

#endif //CHESS_CHESSBOARD_H
