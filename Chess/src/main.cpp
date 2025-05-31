// Chess
#include <limits>
#include "Chess.h"
#include "ChessBoard.h"
#include "Colors.h"
#include "InvalidPieceException.h"

const string YES = "yes";

void showRecommendedMoves(ChessBoard& chessBoard, int playerColor, int depth) {
    PriorityQueue<Move> recommendedMoves = chessBoard.getRecommendedMoves(playerColor, depth, 8);
    cout << recommendedMoves;
}

int main()
{
//	string board = "!###############################################P###############";
	string board = "rnbqkbnrpppppppp################################PPPPPPPPRNBQKBNR";
	Chess a(board);
	int codeResponse = 0;
    int depth;
    cout << "Enter depth: ";
    cin >> depth;
    if (depth == 0) {
        cerr << "Depth value must be positive integer!" << endl;
        return 1;
    }

    // ignore leftover break line character from last input
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string choose;
    cout << "Would you like to run automatic game? Enter yes or enter any other input for normal game: ";
    getline(cin, choose);
    shared_ptr<ChessBoard> chessBoard = make_shared<ChessBoard>(board);
    if (choose == YES) {
        // running automatic game with 8 threads
        a.runAutomaticGame(*chessBoard, depth, 8);
    } else {
        try {
            showRecommendedMoves(*chessBoard, Colors::White, depth);
            string res = a.getInput();
            // true means Player1 is playing, false means Player2 is playing
            bool currentPlayer = true;
            while (res != "exit")
            {
                int playerColor = currentPlayer? Colors::White : Colors::Black;
                // extract the source and destination locations from the user's input 'res'
                int sourceRow = 7 - ('h' - res[0]);
                int destinationRow = 7 - ('h' - res[2]);
                int sourceCol = (res[1] - '0') - 1;
                int destinationCol = (res[3] - '0') - 1;

                pair<const int, const int> validMoveCodes = {41, 42};

                // get the corresponding codeResponse created by ChessBoard object
                codeResponse = chessBoard->getMoveCodeResponse(playerColor, sourceRow, sourceCol, destinationRow, destinationCol, chessBoard->getBoard());
                if (codeResponse == validMoveCodes.first || codeResponse == validMoveCodes.second) {
                    // flipping turns
                    currentPlayer = !currentPlayer;
                }
                a.setCodeResponse(codeResponse);
                showRecommendedMoves(*chessBoard, playerColor, depth);
                res = a.getInput();
            }
        } catch (InvalidPieceException& e) {
            cerr << e.what() << endl;
        }
    }
	cout << endl << "Exiting " << endl;
	return 0;
}