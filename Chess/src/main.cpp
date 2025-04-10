// Chess 
#include "Chess.h"
#include "ChessBoard.h"
#include "Colors.h"

int main()
{
	string board = "R#BQKB#R################################################r#bqkb#r";
//	string board = "##########K###############################R#############r#r#####";
	Chess a(board);
	int codeResponse = 0;
	string res = a.getInput();
    ChessBoard* chessBoard = ChessBoard::getInstance(board);
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
		{
            int sourceRow = 7 - ('h' - res[0]);
            int destinationRow = 7 - ('h' - res[2]);
            int sourceCol = (res[1] - '0') - 1;
            int destinationCol = (res[3] - '0') - 1;
            int playerColor = currentPlayer? Colors::White : Colors::Black;
            codeResponse = chessBoard->getMoveCodeResponse(playerColor, sourceRow, sourceCol, destinationRow, destinationCol);
            if (codeResponse == 41 || codeResponse == 42) {
                currentPlayer = !currentPlayer;
            }
		}
		/**/

		a.setCodeResponse(codeResponse);
		res = a.getInput(); 
	}

	cout << endl << "Exiting " << endl;
    ChessBoard::destroyInstance();
	return 0;
}