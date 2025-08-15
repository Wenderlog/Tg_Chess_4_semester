//
// Created by Кирилл Грибанов  on 21/05/2025.
//

#include "Game_types.h"

bool Coord::operator==(Coord other) const {
    return row == other.row && col == other.col;
}

namespace std {
    size_t hash<Coord>::operator()(Coord coord) const {
        return std::hash<int>()(coord.row) * std::hash<int>()(coord.col);
    }
};