//  Manager.h
//  Chess
//
//  Created by Кирилл Грибанов on 08.12.2024.
//

#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "Table.h"

/*!
 * \class Manager
 * \brief A utility class for managing and interpreting chessboard-related operations.
 * \details The `Manager` class provides functionality to interpret chess moves,
 * validate coordinates, and convert between different coordinate representations.
 * It acts as an intermediary to handle input and verify correctness of chess actions.
 */

class Manager {
public:
    
    /*!
     * \brief Converts a chess move in algebraic notation to board coordinates.
     * \param board The chessboard represented as a 2D vector of shared pointers to `Cell` objects.
     * \param message A string representing the move in algebraic notation (e.g., "e2 e4").
     * \return A pair of coordinates representing the move (from, to). If invalid, returns an empty pair.
     */
    
    std::pair<Coord, Coord> WordToCoord(const std::vector<std::vector<std::shared_ptr<Cell>>>& board, const std::string& message);

    /*!
     * \brief Converts a chess square notation (e.g., "e4") to board coordinates.
     * \param square A string representing the square in standard chess notation.
     * \return A `Coord` structure representing the corresponding row and column on the board.
     */
    
    Coord ConvertToCoord(const std::string& square);

    /*!
     * \brief Validates if a coordinate is within the bounds of the chessboard.
     * \param coord The coordinate to validate.
     * \return `true` if the coordinate is valid (within 0-7 for both row and column), otherwise `false`.
     */
    
    bool IsValidCoord(const Coord& coord);

    /*!
     * \brief Handles white player's castling move (O-O or O-O-O).
     * \param board The chessboard represented as a 2D vector of shared pointers to `Cell` objects.
     * \param message A string representing the castling move (either "O-O" or "O-O-O").
     * \return A pair of coordinates representing the castling move (from, to). If invalid, returns an empty pair.
     */
    
    std::pair<Coord, Coord> HandleWhiteCastle(const std::vector<std::vector<std::shared_ptr<Cell>>>& board, const std::string& message);

    /*!
     * \brief Handles black player's castling move (o-o or o-o-o).
     * \param board The chessboard represented as a 2D vector of shared pointers to `Cell` objects.
     * \param message A string representing the castling move (either "o-o" or "o-o-o").
     * \return A pair of coordinates representing the castling move (from, to). If invalid, returns an empty pair.
     */
    
    std::pair<Coord, Coord> HandleBlackCastle(const std::vector<std::vector<std::shared_ptr<Cell>>>& board, const std::string& message);
};
