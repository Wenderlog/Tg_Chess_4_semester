//
// Created by Кирилл Грибанов  on 21/05/2025.
//

#include "Game_types.h"

/*! \brief
 *   Compares two Coord objects for equality.
 *
 *   The comparison is performed by checking both the row and column values.
 *
 *   \param other The coordinate to compare with the current object.
 *   \return true if both row and column are equal, false otherwise.
 */
bool Coord::operator==(Coord other) const {
    return row == other.row && col == other.col;
}

/*! \brief
 *   Computes the hash value for a Coord object.
 *
 *   The hash is generated based on the row and column values,
 *   enabling Coord to be used as a key in unordered containers
 *   like std::unordered_set or std::unordered_map.
 *
 *   \param coord The coordinate to be hashed.
 *   \return The hash value as size_t.
 */
namespace std {
    size_t hash<Coord>::operator()(Coord coord) const {
        return std::hash<int>()(coord.row) * std::hash<int>()(coord.col);
    }
};