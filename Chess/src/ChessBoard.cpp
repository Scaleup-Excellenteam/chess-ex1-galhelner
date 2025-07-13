#include <mutex>
#include "ChessBoard.h"
#include "Colors.h"
#include "Rook.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Pawn.h"
#include "Knight.h"
#include "InvalidPieceException.h"
#include "ThreadPool.h"

const int EMPTY_SOURCE_CODE = 11;
const int ENEMY_AT_SOURCE_CODE = 12;
const int MATE_AT_DESTINATION_CODE = 13;
const int INVALID_MOVE_OR_BLOCKED_CODE = 21;
const int CHECK_MATE_CODE = 31;
const int VALID_MOVE_CODE = 42;
const int CHECK_VALID_MOVE_CODE = 41;

const int PAWN_SCORE = 100;
const int KNIGHT_SCORE = 320;
const int BISHOP_SCORE = 330;
const int ROOK_SCORE = 500;
const int QUEEN_SCORE = 900;
const int KING_SCORE = 20000;

const int MIN_EVAL_VALUE = -1000000;
const int MAX_EVAL_VALUE = 1000000;

ChessBoard::ChessBoard(): board(vector<vector<ChessPiece*>>(8, vector<ChessPiece*>(8, nullptr))) {}

ChessBoard::ChessBoard(const std::string &boardString) : board(vector<vector<ChessPiece*>>(8, vector<ChessPiece*>(8, nullptr))){
    if (!boardString.empty() && boardString.length() == 64) {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                int symbol_index = (row * 8) + col;
                char symbol = boardString[symbol_index];
                try {
                    addPiece(symbol, col, row);
                } catch (InvalidPieceException& e) {
                    throw e;
                }
            }
        }
    }
}

ChessBoard::~ChessBoard() {
    for (auto& pieces : board) {
        for (auto piece : pieces) {
            delete piece;
        }
        pieces.clear();
    }
    board.clear();
}

vector<vector<ChessPiece *>> &ChessBoard::getBoard() {
    return board;
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
        case 'P': board[row][column] = new Pawn(Colors::White, column, row); break;
        case 'p': board[row][column] = new Pawn(Colors::Black, column, row); break;
        case 'N': board[row][column] = new Knight(Colors::White, column, row); break;
        case 'n': board[row][column] = new Knight(Colors::Black, column, row); break;
        case '#': board[row][column] = nullptr; break;
        default: throw InvalidPieceException("Invalid piece: " + string(1, symbol));
    }
}

int ChessBoard::getMoveCodeResponse(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol, vector<vector<ChessPiece*>>& clonedBoard) {
    ChessPiece* sourcePiece = clonedBoard[sourceRow][sourceCol];
    ChessPiece* destinationPiece = clonedBoard[destinationRow][destinationCol];

    int opponentColor = playerColor == 0 ? 1 : 0;

    if (sourcePiece == nullptr) {
        return EMPTY_SOURCE_CODE;
    }

    if (sourcePiece->getColor() != playerColor) {
        return ENEMY_AT_SOURCE_CODE;
    }

    if (destinationPiece != nullptr && destinationPiece->getColor() == playerColor) {
        return MATE_AT_DESTINATION_CODE;
    }

    if (!sourcePiece->isValidMove(destinationCol, destinationRow, clonedBoard) ||
    isAnyPieceBlocking(sourceRow, sourceCol, destinationRow, destinationCol, clonedBoard)) {
        return INVALID_MOVE_OR_BLOCKED_CODE;
    }

    if (isCheck(playerColor, sourceRow, sourceCol, destinationRow, destinationCol, clonedBoard)) {
        return CHECK_MATE_CODE;
    }

    if (isCheck(opponentColor, sourceRow, sourceCol, destinationRow, destinationCol, clonedBoard)) {
        sourcePiece->setRow(destinationRow);
        sourcePiece->setColumn(destinationCol);
        clonedBoard[sourceRow][sourceCol] = nullptr;
        clonedBoard[destinationRow][destinationCol] = sourcePiece;
        return CHECK_VALID_MOVE_CODE;
    }

    sourcePiece->setRow(destinationRow);
    sourcePiece->setColumn(destinationCol);
    clonedBoard[sourceRow][sourceCol] = nullptr;
    clonedBoard[destinationRow][destinationCol] = sourcePiece;
    return VALID_MOVE_CODE;
}

/**
 * Check if any chess piece is on the path from source to destination.
 * @param sourceRow(int) - row index to move from.
 * @param sourceCol(int) - column index to move from.
 * @param destinationRow(int) - row index to move to.
 * @param destinationCol(int) - column index to move to.
 * @param clonedBoard - cloned structure of the game board.
 * @return bool - true if any chess piece is blocking the path, otherwise false.
 */
bool ChessBoard::isAnyPieceBlocking(int sourceRow, int sourceCol, int destinationRow, int destinationCol, vector<vector<ChessPiece*>>& clonedBoard) {
    auto pathPieces = getPathPieces(sourceRow, sourceCol, destinationRow, destinationCol, clonedBoard);
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
 * @param clonedBoard - cloned structure of the game board.
 * @return vector<ChessPiece*> - a vector of all the chess pieces on the path.
 */
vector<ChessPiece *> ChessBoard::getPathPieces(int sourceRow, int sourceCol, int destinationRow, int destinationCol, vector<vector<ChessPiece*>>& clonedBoard) {
    vector<ChessPiece*> pathPieces;
    if (sourceRow == destinationRow) {
        // moving horizontally
        if (sourceCol < destinationCol) {
            // moving right
            pathPieces = vector<ChessPiece*>(clonedBoard[sourceRow].begin() + sourceCol + 1, clonedBoard[sourceRow].begin() + destinationCol);
        } else {
            // moving left
            pathPieces = vector<ChessPiece*>(clonedBoard[sourceRow].begin() + destinationCol + 1, clonedBoard[sourceRow].begin() + sourceCol);
        }
    } else if (sourceCol == destinationCol){
        // moving vertically
        if (sourceRow < destinationRow) {
            // moving down
            for (int row = sourceRow + 1; row < destinationRow; row++) {
                pathPieces.push_back(clonedBoard[row][sourceCol]);
            }
        } else {
            // moving up
            for (int row = destinationRow + 1; row < sourceRow; row++) {
                pathPieces.push_back(clonedBoard[row][sourceCol]);
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
                    pathPieces.push_back(clonedBoard[sourceRow + i][sourceCol + i]);
                }
            } else {
                // moving diagonally down-left
                int distance = sourceCol - destinationCol;
                for (int i = 1; i < distance; i++) {
                    pathPieces.push_back(clonedBoard[sourceRow + i][sourceCol - i]);
                }
            }
        } else {
            // moving diagonally up
            if (sourceCol < destinationCol) {
                // moving diagonally up-right
                int distance = destinationCol - sourceCol;
                for (int i = 1; i < distance; i++) {
                    pathPieces.push_back(clonedBoard[sourceRow - i][sourceCol + i]);
                }
            } else {
                // moving diagonally up-left
                int distance = sourceCol - destinationCol;
                for (int i = 1; i < distance; i++) {
                    pathPieces.push_back(clonedBoard[sourceRow - i][sourceCol - i]);
                }
            }
        }
    }

    return pathPieces;
}

/**
 * Find the location of the king chess piece by color.
 * @param playerColor(int) - color of the king to find.
 * @param clonedBoard - cloned structure of the game board.
 * @return pair<int,int> - A pair of the king location where first=row and second=column
 */
pair<int, int> ChessBoard::findKing(int playerColor, vector<vector<ChessPiece*>>& clonedBoard) {
    int rows = clonedBoard.size();
    int columns = clonedBoard[0].size();
    pair<int,int> kingLocation = {-1, -1};

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            auto king = dynamic_cast<King*>(clonedBoard[row][col]);
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
 * @param clonedBoard - cloned structure of the game board.
 * @return bool - true if the location is attackable by any opponent piece, otherwise false.
 */
bool ChessBoard::isAttackable(int playerColor, int testedRow, int testedCol, vector<vector<ChessPiece*>>& clonedBoard) {
    int rows = clonedBoard.size();
    int columns = clonedBoard[0].size();

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            ChessPiece* currentPiece = clonedBoard[row][col];
            if (currentPiece != nullptr && currentPiece->getColor() != playerColor) {
                bool isValidMove = currentPiece->isValidMove(testedCol, testedRow, clonedBoard);
                if (isValidMove) {
                    bool isPathClear = !isAnyPieceBlocking(row, col, testedRow, testedCol, clonedBoard);
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
 * @param clonedBoard - cloned structure of the game board.
 * @return bool - true if the move caused a check, otherwise false.
 */
bool ChessBoard::isCheck(int playerColor, int sourceRow, int sourceCol, int destinationRow, int destinationCol, vector<vector<ChessPiece*>>& clonedBoard) {
    // store the board state before move simulation
    ChessPiece* pieceToMove = clonedBoard[sourceRow][sourceCol];
    ChessPiece* pieceToOverride = clonedBoard[destinationRow][destinationCol];

    // simulating the move
    clonedBoard[sourceRow][sourceCol] = nullptr;
    clonedBoard[destinationRow][destinationCol] = pieceToMove;

    if (pieceToMove == nullptr) {
        return false;
    }

    pieceToMove->setRow(destinationRow);
    pieceToMove->setColumn(destinationCol);

    // find the king location
    pair<int,int> kingLocation = findKing(playerColor, clonedBoard);

    if (kingLocation.first == -1 || kingLocation.second == -1) {
        return false;
    }

    // check if after the move simulation the king is attackable
    bool isKingAttackable = isAttackable(playerColor, kingLocation.first, kingLocation.second, clonedBoard);

    // restore the board state as it was before move simulation
    clonedBoard[sourceRow][sourceCol] = pieceToMove;
    clonedBoard[destinationRow][destinationCol] = pieceToOverride;

    pieceToMove->setRow(sourceRow);
    pieceToMove->setColumn(sourceCol);

    return isKingAttackable;
}

/**
 * Find all the possible valid moves from some source location.
 * @param playerColor(int) - the color of the current player.
 * @param sourceRow(int) - row index of the source location.
 * @param sourceCol(int) - column index of the source location.
 * @param clonedBoard(vector<vector<ChessPiece*>>&) - cloned structure of the game board.
 * @param clonedInstance(shared_ptr<ChessBoard>&) - cloned instance of the board object.
 * @return vector<Move> - vector contains all the possible valid moves.
 */
vector<Move> ChessBoard::getValidMoves(int playerColor, int sourceRow, int sourceCol, vector<vector<ChessPiece*>>& clonedBoard, shared_ptr<ChessBoard>& clonedInstance) {
    int rows = clonedBoard.size();
    int columns = clonedBoard[0].size();
    vector<Move> validMoves;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            ChessPiece* pieceToMove = clonedBoard[sourceRow][sourceCol];
            ChessPiece* pieceToOverride = clonedBoard[row][col];

            int responseCode = clonedInstance->getMoveCodeResponse(playerColor, sourceRow, sourceCol, row, col, clonedBoard);
            if (responseCode == VALID_MOVE_CODE || responseCode == CHECK_VALID_MOVE_CODE) {
                // undo move
                clonedBoard[sourceRow][sourceCol] = pieceToMove;
                pieceToMove->setRow(sourceRow);
                pieceToMove->setColumn(sourceCol);
                clonedBoard[row][col] = pieceToOverride;
                if (pieceToOverride != nullptr) {
                    pieceToOverride->setRow(row);
                    pieceToOverride->setColumn(col);
                }
                Move move(sourceRow, sourceCol, row, col);
                validMoves.push_back(move);
            }
        }
    }

    return validMoves;
}

/**
 * Evaluate moves score
 * @param depth(int) - amount of turns to simulate.
 * @param isMaximizingPlayer(bool) - true if the current player is white, otherwise false (maximize for white).
 * @param clonedBoard(shared_ptr<ChessBoard>&) - cloned structure of the game board.
 * @param clonedInstance(shared_ptr<ChessBoard>&) - cloned instance of the board object.
 * @return int - evaluation score.
 */
int ChessBoard::minimax(int depth, bool isMaximizingPlayer, vector<vector<ChessPiece*>>& clonedBoard, shared_ptr<ChessBoard>& clonedInstance) {
    if (depth == 0) {
        return clonedInstance->evaluateBoard(clonedBoard, clonedInstance);
    }

    int bestEval = isMaximizingPlayer ? MIN_EVAL_VALUE : MAX_EVAL_VALUE;
    int playerColor = isMaximizingPlayer ? Colors::White : Colors::Black;

    for (int srcRow = 0; srcRow < 8; srcRow++) {
        for (int srcCol = 0; srcCol < 8; srcCol++) {
            ChessPiece* piece = clonedBoard[srcRow][srcCol];
            if (piece == nullptr || piece->getColor() != playerColor) continue;

            vector<Move> validMoves = clonedInstance->getValidMoves(playerColor, srcRow, srcCol, clonedBoard, clonedInstance);
            for (Move& move : validMoves) {
                ChessPiece* captured = clonedBoard[move.destination.first][move.destination.second];
                ChessPiece* movingPiece = clonedBoard[srcRow][srcCol];

                // moving the piece
                clonedBoard[move.destination.first][move.destination.second] = movingPiece;
                clonedBoard[srcRow][srcCol] = nullptr;
                movingPiece->setRow(move.destination.first);
                movingPiece->setColumn(move.destination.second);

                // calculate scores
                int baseScore = clonedInstance->minimax(depth - 1, !isMaximizingPlayer, clonedBoard, clonedInstance);
                int testedScore = clonedInstance->scoreMove(move, movingPiece, clonedBoard, clonedInstance);
                int totalScore = baseScore + testedScore;

                // undo the move
                clonedBoard[srcRow][srcCol] = movingPiece;
                clonedBoard[move.destination.first][move.destination.second] = captured;
                movingPiece->setRow(srcRow);
                movingPiece->setColumn(srcCol);

                if (isMaximizingPlayer)
                    bestEval = max(bestEval, totalScore);
                else
                    bestEval = min(bestEval, totalScore);
            }
        }
    }
    return bestEval;
}

/**
 * Find all the threats on some target location.
 * @param targetRow(int) - index of the target location row.
 * @param targetCol(int) - index of the target location column.
 * @param attackerColor(int) - color of the opponent.
 * @param clonedBoard(shared_ptr<ChessBoard>&) - cloned structure of the game board.
 * @param clonedInstance(shared_ptr<ChessBoard>&) - cloned instance of the board object.
 * @return vector<pair<int ,int>> - vector contains all the location pairs of the threat pieces.
 */
vector<pair<int, int>> ChessBoard::getAllThreats(int targetRow, int targetCol, int attackerColor, vector<vector<ChessPiece*>>& clonedBoard, shared_ptr<ChessBoard>& clonedInstance) {
    vector<pair<int , int>> attackers;
    int rows = clonedBoard.size();
    int columns = clonedBoard[0].size();

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            ChessPiece* piece = clonedBoard[row][col];
            if (piece && piece->getColor() == attackerColor) {
                if (piece->isValidMove(targetCol, targetRow, clonedBoard) && !clonedInstance->isAnyPieceBlocking(row, col, targetRow, targetCol, clonedBoard)) {
                    attackers.push_back({row, col});
                }
            }
        }
    }
    return attackers;
}

/**
 * Calculate the score of a tested move.
 * @param move(Move&) - the tested move to evaluate its score.
 * @param movingPiece(ChessPiece*) - the piece that moving.
 * @param clonedBoard(shared_ptr<ChessBoard>&) - cloned structure of the game board.
 * @param clonedInstance(shared_ptr<ChessBoard>&) - cloned instance of the board object.
 * @return int - the evaluated score.
 */
int ChessBoard::scoreMove(const Move &move, ChessPiece *movingPiece, vector<vector<ChessPiece*>>& clonedBoard, shared_ptr<ChessBoard>& clonedInstance) {
    int score = 0;
    ChessPiece* captured = clonedBoard[move.destination.first][move.destination.second];

    // moving the piece
    clonedBoard[move.destination.first][move.destination.second] = movingPiece;
    clonedBoard[move.source.first][move.source.second] = nullptr;
    movingPiece->setRow(move.destination.first);
    movingPiece->setColumn(move.destination.second);

    int opponentColor = (movingPiece->getColor() == Colors::White) ? Colors::Black : Colors::White;
    int movedValue = clonedInstance->getPieceValue(movingPiece);

    auto threats = clonedInstance->getAllThreats(move.destination.first, move.destination.second, opponentColor, clonedBoard, clonedInstance);
    for (const auto& pos : threats) {
        ChessPiece* attacker = clonedBoard[pos.first][pos.second];
        if (attacker && getPieceValue(attacker) < movedValue) {
            score -= (movedValue - getPieceValue(attacker)) / 2;
        }
    }

    auto attacks = clonedInstance->getValidMoves(movingPiece->getColor(), move.destination.first, move.destination.second, clonedBoard, clonedInstance);
    for (const auto& pos : attacks) {
        ChessPiece* target = clonedBoard[pos.destination.first][pos.destination.second];
        if (target && clonedInstance->getPieceValue(target) > movedValue && target->getColor() == opponentColor) {
            score += (clonedInstance->getPieceValue(target) - movedValue) / 2;
        }
    }

    if (captured && captured->getColor() == opponentColor) {
        score += clonedInstance->getPieceValue(captured);
    }

    // undo the move
    clonedBoard[move.destination.first][move.destination.second] = captured;
    clonedBoard[move.source.first][move.source.second] = movingPiece;
    movingPiece->setRow(move.source.first);
    movingPiece->setColumn(move.source.second);

    return score;
}

/**
 * Evaluate the whole board score.
 * @param clonedBoard(shared_ptr<ChessBoard>&) - cloned structure of the game board.
 * @param clonedInstance(shared_ptr<ChessBoard>&) - cloned instance of the board object.
 * @return int - evaluation score.
 */
int ChessBoard::evaluateBoard(vector<vector<ChessPiece*>>& clonedBoard, shared_ptr<ChessBoard>& clonedInstance) {
    int score = 0;
    int rows = clonedBoard.size();
    int columns = clonedBoard[0].size();

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            auto piece = clonedBoard[row][col];
            score += clonedInstance->getPieceValue(piece);
        }
    }

    return score;
}

/**
 * Gets the corresponding score value for a given piece.
 * @param piece(ChessPiece*) - the piece to get the score value of.
 * @return int - the score value of the given piece.
 */
int ChessBoard::getPieceValue(ChessPiece* piece) {
    if (piece == nullptr) {
        return 0;
    }

    auto pawn = dynamic_cast<Pawn*>(piece);
    if (pawn) return PAWN_SCORE;
    auto knight = dynamic_cast<Knight*>(piece);
    if (knight) return KNIGHT_SCORE;
    auto bishop = dynamic_cast<Bishop*>(piece);
    if (bishop) return BISHOP_SCORE;
    auto rook = dynamic_cast<Rook*>(piece);
    if (rook) return ROOK_SCORE;
    auto queen = dynamic_cast<Queen*>(piece);
    if (queen) return QUEEN_SCORE;
    auto king = dynamic_cast<King*>(piece);
    if (king) return KING_SCORE;
}

/**
 * Create a new clone of the ChessBoard instance.
 * @return ChessBoard* - clone of 'this' instance.
 */
ChessBoard *ChessBoard::clone() {
    ChessBoard* newBoard = new ChessBoard();

    if (this->board.empty()) {
        return newBoard;
    }

    // deep copy the board
    newBoard->board.resize(this->board.size(), vector<ChessPiece*>(this->board[0].size(), nullptr));
    for (int row = 0; row < board.size(); row++) {
        for (int col = 0; col < board[row].size(); col++) {
            if (board[row][col] != nullptr) {
                newBoard->board[row][col] = board[row][col]->clone();
            }
        }
    }

    return newBoard;
}

PriorityQueue<Move> ChessBoard::getRecommendedMoves(int playerColor, int depth, int numOfThreads) {
    PriorityQueue<Move> recommendedMoves;
    std::mutex queueMutex;
    ThreadPool threadPool(numOfThreads);

    int rows = board.size();
    int columns = board[0].size();

    for (int srcRow = 0; srcRow < rows; srcRow++) {
        for (int srcCol = 0; srcCol < columns; srcCol++) {
            auto piece = this->board[srcRow][srcCol];
            if (piece == nullptr || piece->getColor() != playerColor) continue;

            // create one thread per piece
            threadPool.enqueue([=, this, &queueMutex, &recommendedMoves]() {
                std::shared_ptr<ChessBoard> clonedInstance(this->clone());
                auto& clonedBoard = clonedInstance->board;

                vector<Move> validMoves = clonedInstance->getValidMoves(playerColor, srcRow, srcCol, clonedBoard, clonedInstance);
                for (Move& move : validMoves) {
                    if (move.destination.first < 0 || move.destination.first >= clonedBoard.size() ||
                        move.destination.second < 0 || move.destination.second >= clonedBoard[0].size()) {
                        return; // skip invalid move
                    }

                    // Simulate the move
                    ChessPiece* captured = clonedBoard[move.destination.first][move.destination.second];
                    ChessPiece* movingPiece = clonedBoard[srcRow][srcCol];

                    clonedBoard[move.destination.first][move.destination.second] = movingPiece;
                    clonedBoard[srcRow][srcCol] = nullptr;
                    movingPiece->setRow(move.destination.first);
                    movingPiece->setColumn(move.destination.second);

                    // Evaluate the board
                    int eval = clonedInstance->minimax(depth, playerColor == Colors::White, clonedBoard, clonedInstance);

                    // Undo the move
                    clonedBoard[srcRow][srcCol] = movingPiece;
                    clonedBoard[move.destination.first][move.destination.second] = captured;
                    movingPiece->setRow(srcRow);
                    movingPiece->setColumn(srcCol);

                    // Store the move with its score
                    move.score = eval;

                    // lock before access shared priority queue
                    std::lock_guard<std::mutex> lock(queueMutex);
                    recommendedMoves.push(move);
                }
            });
        }
    }

    // wait for all the threads to finish
    threadPool.wait_for_all();

    // return only the 5 highest score
    PriorityQueue<Move> highestFive;
    for (int i = 0; i < 5; i++) {
        try {
            highestFive.push(recommendedMoves.pull());
        } catch (EmptyQueueException& e) {

        }
    }

    return highestFive;
}
