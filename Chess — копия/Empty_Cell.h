//
//  Empty_Cell.h
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#pragma once
#include "Cell.h"

/*!
 * \class EmptyCell
 * \brief A class representing an empty cell on the chessboard.
 * \details The `EmptyCell` class inherits from the `Cell` class and provides the implementation of the abstract methods
 * to return the color, available moves, and the name of the empty cell. This class models a cell that does not contain any piece.
 */

constexpr char Symbol = '.'; ///< The symbol representing an empty cell.
constexpr std::string_view EmptyName = "Empty"; ///< The name of the empty cell.

class EmptyCell : public Cell {
public:
    
    /*!
     * \brief Returns the color of the empty cell.
     * \details As an empty cell does not have a piece, this function returns the default color of the empty cell.
     * \return Colour The color of the empty cell.
     */
    
    Colour getColour() const override;

    /*!
     * \brief Returns a vector of cells under attack by an empty cell.
     * \details Since the empty cell does not represent any chess piece, it does not have any attacked cells.
     * \return std::vector<Coord> An empty vector indicating no cells are under attack.
     */
    
    std::unordered_set<Coord> getHits() const override;

    /*!
     * \brief Returns the name of the empty cell.
     * \details This method returns the name "Empty" to represent an empty cell.
     * \return std::string The name of the empty cell.
     */
    
    std::string_view Name() const override;

    /*!
     * \brief Returns the reserved steps for the empty cell.
     * \details As an empty cell does not contain any piece, there are no possible moves associated with it.
     * \return std::vector<Coord> An empty vector as no moves are reserved for the empty cell.
     */
    
    std::unordered_set<Coord> getReservedSteps() const override;

    /*!
     * \brief Returns the symbol for the empty cell.
     * \details The empty cell is represented by a period ('.') symbol on the board.
     * \return char The symbol representing an empty cell.
     */
    
    char getSymbol() const override;

    /*!
     * \brief Constructor for the EmptyCell class.
     * \details Initializes the empty cell with its coordinates.
     * \param coord The coordinates of the empty cell.
     */
    
    EmptyCell(Coord coord) : Cell(coord) {}

    /*!
     * \brief Destructor for the EmptyCell class.
     * \details The destructor is defined but does not perform any specific cleanup as there is no dynamic memory allocation.
     */
    
    ~EmptyCell() = default;

protected:
    Colour colour_; ///< The color of the empty cell.
};
