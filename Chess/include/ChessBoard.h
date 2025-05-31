#ifndef CHESS_CHESSBOARD_H
#define CHESS_CHESSBOARD_H
#include <string>
#include <vector>
#include <memory>
#include "ChessPiece.h"
#include "PriorityQueue.h"
#include "Move.h"
using namespace std;

/*
 * A class represent a chess board object
 * stores all the chess pieces on the board and provide a codeResponse for a given move.
 */

class ChessBoard{
private:
    vector<vector<ChessPiece*>> board;
    void addPiece(char symbol, int column, int row);
    bool isAnyPieceBlocking(int sourceRow, int sourceCol, int destinationRow, int destinationCol, vector<vector<ChessPiece*>>& clonedBoard);
    vector<ChessPiece*> getPathPieces(int sourceRow, int sourceCol, int destinationRow, int destinationCol, vector<vector<ChessPiece*>>& clonedBoard);
    pair<int,int> findKing(int playerColor, vector<vector<ChessPiece*>>& clonedBoard);
    bool isAttackable(int playerColor, int testedRow, int testedCol, vector<vector<ChessPiece*>>& clonedBoard);
    bool isCheck(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol, vector<vector<ChessPiece*>>& clonedBoard);
    vector<Move> getValidMoves(int playerColor, int sourceRow, int sourceCol, vector<vector<ChessPiece*>>& clonedBoard, shared_ptr<ChessBoard>& clonedInstance);
    int getPieceValue(ChessPiece* piece);
    int evaluateBoard(vector<vector<ChessPiece*>>& clonedBoard, shared_ptr<ChessBoard>& clonedInstance);
    int minimax(int depth, bool isMaximizingPlayer, vector<vector<ChessPiece*>>& clonedBoard, shared_ptr<ChessBoard>& clonedInstance);
    int scoreMove(const Move& move, ChessPiece* movingPiece, vector<vector<ChessPiece*>>& clonedBoard, shared_ptr<ChessBoard>& clonedInstance);
    vector<pair<int, int>> getAllThreats(int targetRow, int targetCol, int attackerColor, vector<vector<ChessPiece*>>& clonedBoard, shared_ptr<ChessBoard>& clonedInstance);
    ChessBoard* clone();

public:
    ChessBoard();
    ChessBoard(const string& boardString);
    ~ChessBoard();
    /**
     * Analyze the move and provide the corresponding codeResponse for the graphic machine.
     * @param playerColor(int) - color of the current player that is trying to move.
     * @param sourceRow(int) - row index to move from.
     * @param sourceCol(int) - column index to move from.
     * @param destinationRow(int) - row index to move to.
     * @param destinationCol(int) - column index to move to.
     * @param clonedBoard - cloned structure of the game board.
     * @return int - corresponding codeResponse for the graphic machine.
     */
    int getMoveCodeResponse(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol, vector<vector<ChessPiece*>>& clonedBoard);

    /**
     * Calculate the best 5 moves the piece at current location can do.
     * @param playerColor(int) - color of the current player.
     * @param depth(int) - amount of turns to simulate.
     * @param numOfThreads(int) - amount of threads to run concurrently in the algorithm.
     * @return PriorityQueue<int> - the best 5 moves in a priority queue data structure.
     */
    PriorityQueue<Move> getRecommendedMoves(int playerColor, int depth, int numOfThreads);

    vector<vector<ChessPiece*>>& getBoard();
};

#endif //CHESS_CHESSBOARD_H
