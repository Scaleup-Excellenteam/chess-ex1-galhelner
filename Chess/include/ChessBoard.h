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
    bool isChess(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol);
    bool isSelfChess(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol);
    bool isAnyPieceBlocking(int sourceRow, int sourceCol, int destinationRow, int destinationCol);
    vector<ChessPiece*> getPathPieces(int sourceRow, int sourceCol, int destinationRow, int destinationCol);

public:
    ~ChessBoard();
    ChessBoard(const ChessBoard&) = delete;
    ChessBoard& operator= (const ChessBoard&) = delete;
    static ChessBoard* getInstance(const string& boardString);
    static void destroyInstance();
    int getMoveCodeResponse(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol);
};

#endif //CHESS_CHESSBOARD_H
