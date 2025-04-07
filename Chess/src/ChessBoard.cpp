#include "ChessBoard.h"
#include "Colors.h"
#include "Rock.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"

ChessBoard* ChessBoard::instance = nullptr;

ChessBoard::ChessBoard(): board(vector<vector<ChessPiece*>>(8, vector<ChessPiece*>(8, nullptr))) {}

vector<vector<ChessPiece *>> &ChessBoard::getBoard() {
    return board;
}

ChessBoard *ChessBoard::getInstance(const string& boardString) {
    if (instance == nullptr) {
        instance = new ChessBoard();
        if (!boardString.empty() && boardString.length() == 64) {
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    int symbol_index = (row * 8) + col;
                    char symbol = boardString[symbol_index];
                    instance->addPiece(symbol, col, row);
                }
            }
        }
    }
    return instance;
}

void ChessBoard::addPiece(char symbol, int column, int row) {
    Colors colors;
    switch (symbol) {
        case 'R': board[row][column] = new Rock(colors.White, column, row); break;
        case 'r': board[row][column] = new Rock(colors.Black, column, row); break;
        case 'B': board[row][column] = new Bishop(colors.White, column, row); break;
        case 'b': board[row][column] = new Bishop(colors.Black, column, row); break;
        case 'K': board[row][column] = new King(colors.White, column, row); break;
        case 'k': board[row][column] = new King(colors.Black, column, row); break;
        case 'Q': board[row][column] = new Queen(colors.White, column, row); break;
        case 'q': board[row][column] = new Queen(colors.Black, column, row); break;
        default: board[row][column] = nullptr;
    }
}