//
//  Knight_Cell.cpp
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#include "Knight_Cell.h"

KnightCell::KnightCell(Coord coord, Colour colour): Cell(coord), colour_(colour){}

Colour KnightCell::getColour() const {
    return colour_;
}

std::unordered_set<Coord> KnightCell::getReservedSteps() const {
    std::unordered_set<Coord> moves;
    moves.reserve(8);
    
    int drows[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int dcols[] = {-1, 1, -2, 2, -2, 2, -1, 1};
    
    for (int i = 0; i < 8; ++i) {
        int new_row = coord_.row + drows[i];
        int new_col = coord_.col + dcols[i];
        if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8) {
            moves.insert({new_row, new_col});
        }
    }
    return moves;
}

std::unordered_set<Coord> KnightCell::getHits() const {
    return getReservedSteps();
}

std::string_view KnightCell::Name() const {
    return KnightName;
    }

char KnightCell::getSymbol() const {
    return (colour_ == Colour::WHITE) ? 'N' : 'n';
}
