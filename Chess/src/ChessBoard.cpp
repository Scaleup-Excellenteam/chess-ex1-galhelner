#include "ChessBoard.h"

ChessBoard::ChessBoard(): board(vector<vector<ChessPiece*>>(8, vector<ChessPiece*>(8, nullptr))) {}

ChessBoard *ChessBoard::getInstance(const string& boardString) {
    if (instance == nullptr) {
        instance = new ChessBoard();
        // TODO: fill the board with polymorphic ChessPiece objects based on the boardString argument
    }
    return instance;
}