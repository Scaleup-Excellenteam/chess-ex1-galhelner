#pragma once
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif

#include <string>
#include "ChessBoard.h"

using std::cout;
using std::cin; 
using std::endl;
using std::string; 

const int _SIZE = 21;

class Chess {
	unsigned char m_board[_SIZE][_SIZE] = { 0 };
	bool m_turn = true;
	string m_boardString;
	string m_input;
	string m_msg = "\n";
	string m_errorMsg = "\n";
	int m_codeResponse;

	void clear() const;
	void setFrames();
	void setPieces();
	void show() const;
	void displayBoard() const;
	void showAskInput() const;
	bool isSame() const;
	bool isValid() const;
	bool isExit() const;
	void excute();
	void doTurn();
    double measureGameTimeHelper(ChessBoard& chessBoard, int depth, int numOfThreads);

public:
	Chess(const string& start = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr");
	Chess(const Chess&)=delete;
	Chess& operator=(const Chess&) = delete;
	string getInput(istream& in = cin);
	void setCodeResponse(int codeResponse);

    /**
     * Runs the automatic game that use my recommended moves algorithm.
     * @param chessBoard - reference to the ChessBoard object.
     * @param depth - depth value for the recommended moves algorithm.
     * @param numOfThreads - amount of threads to run concurrently in the algorithm.
     */
    void runAutomaticGame(ChessBoard& chessBoard, int depth, int numOfThreads);

    /**
     * Measure the running time of automatic game with 2,4,8 threads.
     * @param board - string representation of the board layout.
     * @param depth - depth value for the recommended moves algorithm.
     */
    void measureGameTimes(const string board, int depth);
};