//
//  Queen_Cell.cpp
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#include "Queen_Cell.h"

QueenCell::QueenCell(Coord coord, Colour colour): Cell(coord), colour_(colour){}

Colour QueenCell::getColour() const {
    return colour_;
}

std::unordered_set<Coord> QueenCell::getReservedSteps() const {
    std::unordered_set<Coord> moves;
    moves.reserve(27);
    
    for(int i = 1; i <= 7; ++i) {
        if(coord_.row + i <= 7) moves.insert({coord_.row + i, coord_. col});
        if(coord_.row >= i) moves.insert({coord_.row - i, coord_.col});
        if(coord_.col + i <= 7) moves.insert({coord_.row, coord_. col + i});
        if(coord_.col >= i) moves.insert({coord_.row, coord_.col - i});
    }
    
    
    for(int i = 1; i < 8; ++i) {
        
        if(coord_.row + i < 8 && coord_.col + i < 8) moves.insert({coord_.row + i, coord_.col + i});
        
        if(coord_.row >= i && coord_.col  >= i) moves.insert({coord_.row - i, coord_.col - i});
        
        if(coord_.col + i < 8  && coord_.row >= i) moves.insert({coord_.row - i, coord_. col + i});
        
        if(coord_.col >= i  && coord_.row + i < 8) moves.insert({coord_.row + i, coord_.col - i});
    }
    return moves;
}

std::unordered_set<Coord> QueenCell::getHits() const {
    return getReservedSteps();
}

std::string_view QueenCell::Name() const {
    return QueenName;
}

char QueenCell::getSymbol() const {
    return (colour_ == Colour::WHITE) ? 'Q' : 'q';
}
