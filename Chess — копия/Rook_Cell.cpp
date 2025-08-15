//
//  Rook_Cell.cpp
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#include "Rook_Cell.h"

RookCell::RookCell(Coord coord, Colour colour): Cell(coord), colour_(colour){}

Colour RookCell::getColour() const {
    return colour_;
}

std::unordered_set<Coord> RookCell::getReservedSteps() const {
    std::unordered_set<Coord> moves;
    moves.reserve(14);
    
    for(int i = 1; i <= 7; ++i) {
        if(coord_.row + i <= 7) moves.insert({coord_.row + i, coord_. col});
        if(coord_.row >= i) moves.insert({coord_.row - i, coord_.col});
        if(coord_.col + i <= 7) moves.insert({coord_.row, coord_. col + i});
        if(coord_.col >= i) moves.insert({coord_.row, coord_.col - i});
    }
    return moves;
}

std::unordered_set<Coord> RookCell::getHits() const {
    return getReservedSteps();
}



std::string_view RookCell::Name() const {
    return RookName;
}

char RookCell::getSymbol() const {
    return (colour_ == Colour::WHITE) ? 'R' : 'r';
}

bool RookCell::hasMoved() const {
    return hasMoved_;
}

void RookCell::markMoved() {
    hasMoved_ = true;
}
