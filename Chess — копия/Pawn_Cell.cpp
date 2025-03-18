//
//  Pawn_Cell.cpp
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#include "Pawn_Cell.h"

PawnCell::PawnCell(Coord coord, Colour colour): Cell(coord), colour_(colour){}

Colour PawnCell::getColour() const {
    return colour_;
}

std::unordered_set<Coord> PawnCell::getHits() const {
    std::unordered_set<Coord> Hits;
    if (colour_ == Colour::BLACK) {
        if(coord_.row > 0 && coord_.col > 0){
            Hits.insert({coord_.row - 1, coord_.col - 1});
        }
        if(coord_.row > 0 && coord_.col < 7) {
            Hits.insert({coord_.row - 1, coord_.col + 1});
        }
    }
    else {
        if(coord_.row < 7 && coord_.col > 0){
            Hits.insert({coord_.row + 1, coord_.col - 1});
        }
        if(coord_.row < 7 && coord_.col < 7) {
            Hits.insert({coord_.row + 1, coord_.col + 1});
        }
    }
    return Hits;
}

std::unordered_set<Coord> PawnCell::getReservedSteps() const{
    std::unordered_set<Coord> moves;
    
    if (colour_ == Colour::BLACK) {
        if(coord_.row > 0){
            moves.insert({coord_.row - 1, coord_.col});
        }
        if(coord_.row == 6) {
            moves.insert({coord_.row - 2, coord_.col});
        }
    }
    else {
        if(coord_.row < 7){
            moves.insert({coord_.row + 1, coord_.col});
        }
        if(coord_.row == 1) {
            moves.insert({coord_.row + 2, coord_.col});
        }
    }
    return moves;
}

std::string_view PawnCell::Name() const {
    return PawnName;
}

char PawnCell::getSymbol() const {
    return (colour_ == Colour::WHITE) ? 'P' : 'p';
}
