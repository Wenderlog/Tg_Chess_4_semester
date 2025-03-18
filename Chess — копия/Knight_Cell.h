//
//  Knight_Cell.h
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#pragma once
#include "Cell.h"

constexpr std::string_view KnightName = "Knight";

/*!
 \class KnightCell
 \brief Represents a knight piece on a chessboard.
 \details Models the knight’s unique movement and behavior on an 8x8 chessboard.
 The knight moves in an "L" shape, consisting of two squares in one direction (horizontal or vertical)
 followed by one square perpendicular to that direction. It can jump over other pieces, making it a versatile piece.
 This class provides methods to determine valid moves, attacked cells, and retrieve its name and symbol.
 */

class KnightCell : public Cell {
public:
    /*!
     \brief Constructor to create a knight piece.
     \param coord The initial coordinates of the knight on the board.
     \param colour The color of the knight (white or black).
     */
    
    KnightCell(Coord coord, Colour colour);

    /*!
     \brief Returns the color of the knight.
     \details Determines whether the knight is white or black.
     \return The color of the knight as a `Colour` enumeration.
     */
    
    Colour getColour() const override;

    /*!
     \brief Retrieves the list of cells attacked by the knight.
     \details Calculates all cells the knight can attack, based on its unique "L"-shaped movement pattern.
     The knight's attacks are not obstructed by other pieces.
     \return A vector of coordinates representing the cells the knight attacks.
     */
    
    std::unordered_set<Coord> getHits() const override;

    /*!
     \brief Returns the name of the piece.
     \details For this class, it always returns "Knight".
     \return The name of the piece as a string.
     */
    
    std::string_view Name() const override;

    /*!
     \brief Retrieves the list of valid moves for the knight.
     \details Determines all possible moves for the knight based on its "L"-shaped movement, constrained
     only by the edges of the board. The knight’s ability to jump over other pieces ensures its moves are
     unaffected by obstructions.
     \return A vector of coordinates representing the cells the knight can move to.
     */
    
    std::unordered_set<Coord> getReservedSteps() const override;

    /*!
     \brief Returns the symbol representing the knight on the board.
     \details The symbol is 'N' for a white knight and 'n' for a black knight.
     \return The character symbol for the knight.
     */
    
    char getSymbol() const override;

protected:
    Colour colour_; ///< The color of the knight (white or black).
};
