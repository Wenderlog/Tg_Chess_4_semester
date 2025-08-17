//
// Created by Кирилл Грибанов  on 21/05/2025.
//

#pragma once

#include <cstddef>
#include <string>
#include <unordered_set>
#include <vector>

/*! \brief
 *   Represents the possible colours of chess pieces and cells.
 */
enum class Colour {
    BLACK, ///< Black colour.
    WHITE  ///< White colour.
};

/*! \brief
 *   Represents a coordinate on the chessboard.
 *
 *   Consists of row and column indices. Default values are set to 8.
 */
struct Coord {
    int row = 8; ///< The row index of the board (default 8).
    int col = 8; ///< The column index of the board (default 8).

    /*! \brief
     *   Compares two coordinates for equality.
     *
     *   \param other The coordinate to compare with.
     *   \return true if both row and column are equal, false otherwise.
     */
    bool operator == (Coord other) const;
};

/*! \brief
 *   Specialization of std::hash for the Coord structure.
 *
 *   Allows the Coord type to be used in hash-based containers,
 *   such as std::unordered_set or std::unordered_map.
 */
namespace std {
    template <>
    class hash<Coord> {
    public:
        /*! \brief
         *   Computes the hash value of a coordinate.
         *
         *   \param coord The coordinate to be hashed.
         *   \return The hash value as size_t.
         */
        size_t operator()(Coord coord) const;
    };
}