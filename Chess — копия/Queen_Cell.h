//  Queen_Cell.h
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#pragma once

#include "Cell.h"

constexpr std::string_view QueenName = "Queen";

/*!
 \class QueenCell
 \brief Represents a queen piece on a chessboard.
 \details Models the queen's behavior on an 8x8 chessboard.
 The queen is one of the most powerful pieces in chess, capable of moving any number of squares
 vertically, horizontally, or diagonally. This class includes methods to determine the queen's
 possible moves, cells it can attack, and its general properties.
 */

class QueenCell : public Cell {
public:
    /*!
     \brief Constructor to create a queen piece.
     \param coord The initial coordinates of the queen on the board.
     \param colour The color of the queen (white or black).
     */
    QueenCell(Coord coord, Colour colour);

    /*!
     \brief Returns the color of the queen.
     \return The color of the queen as a `Colour` enumeration.
     */
    Colour getColour() const override;

    /*!
     \brief Retrieves the list of cells attacked by the queen.
     \details Calculates all cells the queen can attack, which includes any square along its
     vertical, horizontal, and diagonal paths until blocked by another piece or the edge of the board.
     \return A set of coordinates representing the cells the queen can attack.
     */
    std::unordered_set<Coord> getHits() const override;

    /*!
     \brief Returns the name of the piece.
     \details For this class, it always returns "Queen".
     \return The name of the piece as a `std::string_view`.
     */
    std::string_view Name() const override;

    /*!
     \brief Retrieves the list of valid moves for the queen.
     \details Determines the queen's possible moves based on its position on the board,
     following its movement rules: unlimited squares vertically, horizontally, or diagonally
     until encountering an obstacle (another piece or the edge of the board).
     \return A set of coordinates representing the cells the queen can move to.
     */
    std::unordered_set<Coord> getReservedSteps() const override;

    /*!
     \brief Returns the symbol representing the queen on the board.
     \details The symbol is 'Q' for a white queen and 'q' for a black queen.
     \return The character symbol for the queen.
     */
    char getSymbol() const override;

protected:
    Colour colour_; ///< The color of the queen (white or black).
};
