#ifndef CHESS_CHESSBOARD_H
#define CHESS_CHESSBOARD_H
#include <string>
#include <vector>
#include "ChessPiece.h"
using namespace std;

/*
 * Singleton class represent a chess board object
 * stores all the chess pieces on the board and provide a codeResponse for a given move.
 */

class ChessBoard {
private:
    static ChessBoard* instance;
    ChessBoard();
    vector<vector<ChessPiece*>> board;
    void addPiece(char symbol, int column, int row);
    bool isAnyPieceBlocking(int sourceRow, int sourceCol, int destinationRow, int destinationCol);
    vector<ChessPiece*> getPathPieces(int sourceRow, int sourceCol, int destinationRow, int destinationCol);
    pair<int,int> findKing(int playerColor);
    bool isAttackable(int playerColor, int testedRow, int testedCol);
    bool isCheck(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol);

public:
    ~ChessBoard();
    ChessBoard(const ChessBoard&) = delete;
    ChessBoard& operator= (const ChessBoard&) = delete;
    static ChessBoard* getInstance(const string& boardString);
    static void destroyInstance();
    /**
     * Analyze the move and provide the corresponding codeResponse for the graphic machine.
     * @param playerColor(int) - color of the current player that is trying to move.
     * @param sourceRow(int) - row index to move from.
     * @param sourceCol(int) - column index to move from.
     * @param destinationRow(int) - row index to move to.
     * @param destinationCol(int) - column index to move to.
     * @return int - corresponding codeResponse for the graphic machine.
     */
    int getMoveCodeResponse(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol);
};

#endif //CHESS_CHESSBOARD_H
