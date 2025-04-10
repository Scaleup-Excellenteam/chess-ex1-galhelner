#include "ChessPiece.h"

/*
 * ChessPiece class implementation
 */

ChessPiece::ChessPiece(int color, int column, int row): color(color), column(column), row(row) {}

int ChessPiece::getColor() {
    return color;
}