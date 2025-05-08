#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H
#include <iostream>
using namespace std;

/*
 * A struct represent a chess move from some source location
 * to destination location with some evaluation score.
 */
struct Move {
    pair<int, int> source;
    pair<int, int> destination;
    int score;

    Move(int srcRow, int srcCol, int destRow, int destCol) {
        source.first = srcRow;
        source.second = srcCol;
        destination.first = destRow;
        destination.second = destCol;
    }

    bool operator>(const Move& other) const {
        return score > other.score;
    }
};

#endif //CHESS_MOVE_H
