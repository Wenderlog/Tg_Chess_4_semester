//
//  Cell_File.h
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#include "Empty_Cell.h"

Colour EmptyCell::getColour() const {
    return Colour::WHITE;
}

std::unordered_set<Coord> EmptyCell::getReservedSteps() const{
    return {};
}

std::unordered_set<Coord> EmptyCell::getHits() const {
    return {};
}

std::string_view EmptyCell::Name() const {
    return EmptyName;
}

char EmptyCell::getSymbol() const {
    return Symbol;
}

