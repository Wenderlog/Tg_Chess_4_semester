//  Pawn_Cell.h
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#pragma once

#include "Cell.h"

constexpr std::string_view PawnName = "Pawn";

/*!
 \class PawnCell
 \brief Represents a pawn piece on a chessboard.
 \details Inherits from the `Cell` class and implements methods specific to the pawn's behavior,
 including determining its available moves, attacked cells, and name.
 */

class PawnCell : public Cell {
public:
    /*!
     \brief Constructor to create a pawn object.
     \param coord The initial coordinates of the pawn on the board.
     \param colour The color of the pawn (white or black).
     */
    
    PawnCell(Coord coord, Colour colour);

    /*!
     \brief Returns the color of the pawn.
     \return The color of the pawn as a `Colour` enumeration.
     */
    
    Colour getColour() const override;

    /*!
     \brief Retrieves the list of cells attacked by the pawn.
     \details The pawn attacks diagonally forward based on its color and current position.
     \return A set of coordinates representing the cells the pawn can attack.
     */
    
    std::unordered_set<Coord> getHits() const override;

    /*!
     \brief Returns the name of the piece.
     \details For this class, it always returns "Pawn".
     \return The name of the piece as a `std::string_view`.
     */
    
    std::string_view Name() const override;

    /*!
     \brief Retrieves the list of valid moves for the pawn.
     \details Calculates the pawn's possible moves based on its position, including a double move
     when the pawn is in its initial position and valid single forward moves.
     \return A set of coordinates representing the cells the pawn can move to.
     */
    
    std::unordered_set<Coord> getReservedSteps() const override;

    /*!
     \brief Returns the symbol representing the pawn on the board.
     \details The symbol is 'P' for a white pawn and 'p' for a black pawn.
     \return The character symbol for the pawn.
     */
    
    char getSymbol() const override;

protected:
    Colour colour_; ///< The color of the pawn (white or black).
};
