#include "ChessBoard.h"
#include "Colors.h"
#include "Rook.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Pawn.h"
#include "Knight.h"

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
        // call the destructor to delete all the pieces in the board.
        delete instance;
        instance = nullptr;
    }
}

ChessBoard *ChessBoard::getInstance(const string& boardString) {
    // provides a singleton design pattern
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

/**
 * Adds a single chess piece at specific location to the board.
 * @param symbol(char) - symbol of the piece on GUI.
 * @param column(int) - piece's location column.
 * @param row(int) - piece's location row.
 */
void ChessBoard::addPiece(char symbol, int column, int row) {
    switch (symbol) {
        case 'R': board[row][column] = new Rook(Colors::White, column, row); break;
        case 'r': board[row][column] = new Rook(Colors::Black, column, row); break;
        case 'B': board[row][column] = new Bishop(Colors::White, column, row); break;
        case 'b': board[row][column] = new Bishop(Colors::Black, column, row); break;
        case 'K': board[row][column] = new King(Colors::White, column, row); break;
        case 'k': board[row][column] = new King(Colors::Black, column, row); break;
        case 'Q': board[row][column] = new Queen(Colors::White, column, row); break;
        case 'q': board[row][column] = new Queen(Colors::Black, column, row); break;
        case 'P': board[row][column] = new Pawn(Colors::White, column, row, board); break;
        case 'p': board[row][column] = new Pawn(Colors::Black, column, row, board); break;
        case 'N': board[row][column] = new Knight(Colors::White, column, row); break;
        case 'n': board[row][column] = new Knight(Colors::Black, column, row); break;
        default: board[row][column] = nullptr;
    }
}

int ChessBoard::getMoveCodeResponse(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol) {
    ChessPiece* sourcePiece = board[sourceRow][sourceCol];
    ChessPiece* destinationPiece = board[destinationRow][destinationCol];

    int opponentColor = playerColor == 0 ? 1 : 0;

    if (sourcePiece == nullptr) {
        return 11;
    }

    if (sourcePiece->getColor()  != playerColor) {
        return 12;
    }

    if (destinationPiece != nullptr && destinationPiece->getColor() == playerColor) {
        return 13;
    }

    if (!sourcePiece->isValidMove(destinationCol, destinationRow) || isAnyPieceBlocking(sourceRow, sourceCol, destinationRow, destinationCol)) {
        return 21;
    }

    if (isCheck(playerColor, sourceRow, sourceCol, destinationRow, destinationCol)) {
        return 31;
    }

    if (isCheck(opponentColor, sourceRow, sourceCol, destinationRow, destinationCol)) {
        sourcePiece->setRow(destinationRow);
        sourcePiece->setColumn(destinationCol);
        board[sourceRow][sourceCol] = nullptr;
        board[destinationRow][destinationCol] = sourcePiece;
        return 41;
    }

    sourcePiece->setRow(destinationRow);
    sourcePiece->setColumn(destinationCol);
    board[sourceRow][sourceCol] = nullptr;
    board[destinationRow][destinationCol] = sourcePiece;
    return 42;
}

/**
 * Check if any chess piece is on the path from source to destination.
 * @param sourceRow(int) - row index to move from.
 * @param sourceCol(int) - column index to move from.
 * @param destinationRow(int) - row index to move to.
 * @param destinationCol(int) - column index to move to.
 * @return bool - true if any chess piece is blocking the path, otherwise false.
 */
bool ChessBoard::isAnyPieceBlocking(int sourceRow, int sourceCol, int destinationRow, int destinationCol) {
    auto pathPieces = getPathPieces(sourceRow, sourceCol, destinationRow, destinationCol);
    // check if the movement path is clear or blocked by some piece
    for (auto piece : pathPieces) {
        if (piece != nullptr) {
            return true;
        }
    }
    return false;
}

/**
 * Find all the chess pieces on the path from source to destination.
 * @param sourceRow(int) - row index to move from.
 * @param sourceCol(int) - column index to move from.
 * @param destinationRow(int) - row index to move to.
 * @param destinationCol(int) - column index to move to.
 * @return vector<ChessPiece*> - a vector of all the chess pieces on the path.
 */
vector<ChessPiece *> ChessBoard::getPathPieces(int sourceRow, int sourceCol, int destinationRow, int destinationCol) {
    vector<ChessPiece*> pathPieces;
    if (sourceRow == destinationRow) {
        // moving horizontally
        if (sourceCol < destinationCol) {
            // moving right
            pathPieces = vector<ChessPiece*>(board[sourceRow].begin() + sourceCol + 1, board[sourceRow].begin() + destinationCol);
        } else {
            // moving left
            pathPieces = vector<ChessPiece*>(board[sourceRow].begin() + destinationCol + 1, board[sourceRow].begin() + sourceCol);
        }
    } else if (sourceCol == destinationCol){
        // moving vertically
        if (sourceRow < destinationRow) {
            // moving down
            for (int row = sourceRow + 1; row < destinationRow; row++) {
                pathPieces.push_back(board[row][sourceCol]);
            }
        } else {
            // moving up
            for (int row = destinationRow + 1; row < sourceRow; row++) {
                pathPieces.push_back(board[row][sourceCol]);
            }
        }
    } else {
        // moving diagonally
        if (sourceRow < destinationRow) {
            // moving diagonally down
            if (sourceCol < destinationCol) {
                // moving diagonally down-right
                int distance = destinationCol - sourceCol;
                for (int i = 1; i < distance; i++) {
                    pathPieces.push_back(board[sourceRow + i][sourceCol + i]);
                }
            } else {
                // moving diagonally down-left
                int distance = sourceCol - destinationCol;
                for (int i = 1; i < distance; i++) {
                    pathPieces.push_back(board[sourceRow + i][sourceCol - i]);
                }
            }
        } else {
            // moving diagonally up
            if (sourceCol < destinationCol) {
                // moving diagonally up-right
                int distance = destinationCol - sourceCol;
                for (int i = 1; i < distance; i++) {
                    pathPieces.push_back(board[sourceRow - i][sourceCol + i]);
                }
            } else {
                // moving diagonally up-left
                int distance = sourceCol - destinationCol;
                for (int i = 1; i < distance; i++) {
                    pathPieces.push_back(board[sourceRow - i][sourceCol - i]);
                }
            }
        }
    }

    return pathPieces;
}

/**
 * Find the location of the king chess piece by color.
 * @param playerColor(int) - color of the king to find.
 * @return pair<int,int> - A pair of the king location where first=row and second=column
 */
pair<int, int> ChessBoard::findKing(int playerColor) {
    int rows = board.size();
    int columns = board[0].size();
    pair<int,int> kingLocation = {-1, -1};

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            auto king = dynamic_cast<King*>(board[row][col]);
            if (king && king->getColor() == playerColor) {
                kingLocation.first = row;
                kingLocation.second = col;
            }
        }
    }
    return kingLocation;
}

/**
 * Check if a tested location on the board is attackable by any opponent piece.
 * @param playerColor(int) - color of the current player.
 * @param testedRow(int) - row index of the tested location.
 * @param testedCol(int) - column index of the tested location.
 * @return bool - true if the location is attackable by any opponent piece, otherwise false.
 */
bool ChessBoard::isAttackable(int playerColor, int testedRow, int testedCol) {
    int rows = board.size();
    int columns = board[0].size();

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            ChessPiece* currentPiece = board[row][col];
            if (currentPiece != nullptr && currentPiece->getColor() != playerColor) {
                bool isValidMove = currentPiece->isValidMove(testedCol, testedRow);
                if (isValidMove) {
                    bool isPathClear = !isAnyPieceBlocking(row, col, testedRow, testedCol);
                    if (isPathClear) {
                        // an opponent piece is able to move on a clear path straight to the tested location
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/**
 * Check if the move from source to destination will cause a check.
 * @param playerColor(int) - color of the current player.
 * @param sourceRow(int) - row index to move from.
 * @param sourceCol(int) - column index to move from.
 * @param destinationRow(int) - row index to move to.
 * @param destinationCol(int) - column index to move to.
 * @return bool - true if the move caused a check, otherwise false.
 */
bool ChessBoard::isCheck(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol) {
    // store the board state before move simulation
    ChessPiece* pieceToMove = board[sourceRow][sourceCol];
    ChessPiece* pieceToOverride = board[destinationRow][destinationCol];

    // simulating the move
    board[sourceRow][sourceCol] = nullptr;
    board[destinationRow][destinationCol] = pieceToMove;
    pieceToMove->setRow(destinationRow);
    pieceToMove->setColumn(destinationCol);

    // find the king location
    pair<int,int> kingLocation = findKing(playerColor);

    // check if after the move simulation the king is attackable
    bool isKingAttackable = isAttackable(playerColor, kingLocation.first, kingLocation.second);

    // restore the board state as it was before move simulation
    board[sourceRow][sourceCol] = pieceToMove;
    board[destinationRow][destinationCol] = pieceToOverride;
    pieceToMove->setRow(sourceRow);
    pieceToMove->setColumn(sourceCol);

    return isKingAttackable;
}