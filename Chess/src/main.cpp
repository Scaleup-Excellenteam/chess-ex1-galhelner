// Chess 
#include "Chess.h"
#include "ChessBoard.h"
#include "Colors.h"

void showRecommendedMoves(ChessBoard& chessBoard, int playerColor, int depth) {
    PriorityQueue<Move> recommendedMoves = chessBoard.getRecommendedMoves(playerColor, depth);
    cout << recommendedMoves;
}

int main()
{
//	string board = "################################################P###############";
	string board = "rnbqkbnrpppppppp################################PPPPPPPPRNBQKBNR";
	Chess a(board);
	int codeResponse = 0;
    ChessBoard* chessBoard = ChessBoard::getInstance(board);
    showRecommendedMoves(*chessBoard, Colors::White, 3);
    string res = a.getInput();
    // true means Player1 is playing, false means Player2 is playing
    bool currentPlayer = true;
	while (res != "exit")
	{
		/*
		codeResponse value :
		Illegal movements :
		11 - there is not piece at the source
		12 - the piece in the source is piece of your opponent
		13 - there one of your pieces at the destination
		21 - illegal movement of that piece
		31 - this movement will cause you checkmate

		legal movements :
		41 - the last movement was legal and cause check
		42 - the last movement was legal, next turn
		*/

		/**/
        int playerColor = currentPlayer? Colors::White : Colors::Black;
		{
            // extract the source and destination locations from the user's input 'res'
            int sourceRow = 7 - ('h' - res[0]);
            int destinationRow = 7 - ('h' - res[2]);
            int sourceCol = (res[1] - '0') - 1;
            int destinationCol = (res[3] - '0') - 1;

            pair<const int, const int> validMoveCodes = {41, 42};

            // get the corresponding codeResponse created by ChessBoard object
            codeResponse = chessBoard->getMoveCodeResponse(playerColor, sourceRow, sourceCol, destinationRow, destinationCol);
            if (codeResponse == validMoveCodes.first || codeResponse == validMoveCodes.second) {
                // flipping turns
                currentPlayer = !currentPlayer;
            }
		}
		/**/

		a.setCodeResponse(codeResponse);
        showRecommendedMoves(*chessBoard, playerColor, 3);
		res = a.getInput();
	}

	cout << endl << "Exiting " << endl;
    ChessBoard::destroyInstance();
	return 0;
}