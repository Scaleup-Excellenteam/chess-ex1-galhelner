#include "ChessPiece.h"

ChessPiece::ChessPiece(int color, int column, int row): color(color), column(column), row(row) {}

int ChessPiece::getColor() {
    return color;
}