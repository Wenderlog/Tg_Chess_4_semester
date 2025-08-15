//
//  Bishop_Cell.h
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#pragma once
#include "Cell.h"

constexpr std::string_view BishopName = "Bishop";

/*!
 \class BishopCell
 \brief Represents a bishop piece on a chessboard.
 \details Models the bishop's behavior on an 8x8 chessboard.
 The bishop moves diagonally across the board for any number of squares, provided its path is unobstructed.
 This class provides methods for determining valid moves, attacked cells, and retrieving its symbol and name.
 */

class BishopCell : public Cell {
public:
    /*!
     \brief Constructor to create a bishop piece.
     \param coord The initial coordinates of the bishop on the board.
     \param colour The color of the bishop (white or black).
     */
    
    BishopCell(Coord coord, Colour colour);

    /*!
     \brief Returns the color of the bishop.
     \details Determines whether the bishop is white or black.
     \return The color of the bishop as a `Colour` enumeration.
     */
    
    Colour getColour() const override;

    /*!
     \brief Retrieves the list of cells attacked by the bishop.
     \details Calculates all cells the bishop can attack, which are all diagonally reachable cells
     from its current position, stopping at the edge of the board or the first encountered piece.
     \return A vector of coordinates representing the cells the bishop attacks.
     */
    
    std::unordered_set<Coord> getHits() const override;

    /*!
     \brief Returns the name of the piece.
     \details For this class, it always returns "Bishop".
     \return The name of the piece as a string.
     */
    
    std::string_view Name() const override;

    /*!
     \brief Retrieves the list of valid moves for the bishop.
     \details Determines all possible moves for the bishop based on its diagonal movement.
     Movement is constrained by the edges of the board and any pieces blocking its path.
     \return A vector of coordinates representing the cells the bishop can move to.
     */
    
    std::unordered_set<Coord> getReservedSteps() const override;

    /*!
     \brief Returns the symbol representing the bishop on the board.
     \details The symbol is 'B' for a white bishop and 'b' for a black bishop.
     \return The character symbol for the bishop.
     */
    
    char getSymbol() const override;

protected:
    Colour colour_; ///< The color of the bishop (white or black).
};
