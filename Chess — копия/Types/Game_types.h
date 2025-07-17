//
// Created by Кирилл Грибанов  on 21/05/2025.
//

#pragma once

#include <unordered_set>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

enum class Colour { BLACK, WHITE };

struct Coord {
    int row = 8;
    int col = 8;
    bool operator==(Coord other) const;
};

namespace std {
    template <>
    class hash<Coord> {
    public:
        size_t operator()(Coord coord) const;
    };
}