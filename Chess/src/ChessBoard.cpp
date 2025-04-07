#include "ChessBoard.h"
#include "Colors.h"
#include "Rock.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"

ChessBoard* ChessBoard::instance = nullptr;

ChessBoard::ChessBoard(): board(vector<vector<ChessPiece*>>(8, vector<ChessPiece*>(8, nullptr))) {}

ChessBoard::~ChessBoard() {
    for (auto& pieces : board) {
        for (auto piece : pieces) {
            delete piece;
        }
        pieces.clear();
    }
    board.clear();
}

void ChessBoard::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

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

int ChessBoard::getMoveCodeResponse(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol) {
    ChessPiece* sourcePiece = board[sourceRow][sourceCol];
    ChessPiece* destinationPiece = board[destinationRow][destinationCol];

    if (sourcePiece == nullptr) {
        return 11;
    }

    if (sourcePiece->getColor()  != playerColor) {
        return 12;
    }

    if (destinationPiece->getColor() == playerColor) {
        return 13;
    }

    if (!sourcePiece->isValidMove(destinationCol, destinationRow) || isAnyPieceBlocking()) {
        return 21;
    }

    if (isSelfChess()) {
        return 31;
    }

    if (isChess()) {
        return 41;
    }

    return 42;
}

bool ChessBoard::isChess() {
    // TODO: check if there is a chess (in the destination row or col there is a sequence of nulls and then an opponent king).
}

bool ChessBoard::isSelfChess() {
    // TODO: check if there is a self chess (in the source row or col there is opponent piece and current player king).
}

bool ChessBoard::isAnyPieceBlocking() {
    // TODO: check whats the meaning of piece that blocking the way (consider implement it for each Piece derived class).
}