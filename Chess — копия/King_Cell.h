//  King_Cell.h
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#pragma once

#include "Cell.h"
#include "Empty_Cell.h"
#include "Rook_Cell.h"
#include <string_view>

constexpr std::string_view KingName = "King";

/*!
 \class KingCell
 \brief Represents a king piece on a chessboard.
 \details Models the king's behavior on an 8x8 chessboard.
 The king is the most important piece in chess, capable of moving one square in any direction
 (vertically, horizontally, or diagonally). This class provides methods for determining valid moves,
 verifying castling conditions, and checking if the king is under attack.
 */

class KingCell : public Cell {
 public:
  /*!
   \brief Constructor to create a king piece.
   \param coord The initial coordinates of the king on the board.
   \param colour The color of the king (white or black).
   */
  KingCell(Coord coord, Colour colour);

  /*!
   \brief Returns the color of the king.
   \return The color of the king as a `Colour` enumeration.
   */
  Colour getColour() const override;

  /*!
   \brief Retrieves the list of cells attacked by the king.
   \details Calculates all cells the king can attack, which includes any square adjacent
   to the king's current position (one square in any direction).
   \return A set of coordinates representing the cells the king can attack.
   */
  std::unordered_set<Coord> getHits() const override;

  /*!
   \brief Returns the name of the piece.
   \details For this class, it always returns "King".
   \return The name of the piece as a `std::string_view`.
   */
  std::string_view Name() const override;

  /*!
   \brief Retrieves the list of valid moves for the king.
   \details Determines all possible moves for the king based on its current position. This includes
   any adjacent square (one square in any direction) provided the square is not occupied by a friendly piece.
   \return A set of coordinates representing the cells the king can move to.
   */
  std::unordered_set<Coord> getReservedSteps() const override;

  /*!
   \brief Returns the symbol representing the king on the board.
   \details The symbol is 'K' for a white king and 'k' for a black king.
   \return The character symbol for the king.
   */
  char getSymbol() const override;

  /*!
   \brief Checks if castling is possible for the king.
   \details Verifies whether the king can perform castling (either short or long)
   based on its movement status, the state of the board, and whether the involved squares
   are under attack.
   \param isShort A boolean indicating if the check is for short castling (true) or long castling (false).
   \param board A reference to the chessboard.
   \return `true` if castling is possible; otherwise, `false`.
   */
  bool canCastle(bool isShort, const std::vector<std::vector<std::shared_ptr<Cell>>>& board) const;

  /*!
   \brief Performs castling for the king.
   \details Moves the king and the rook involved in the castling to their respective positions.
   This function assumes that the castling conditions have already been verified.
   \param isShort A boolean indicating if the move is short castling (true) or long castling (false).
   \param board A reference to the chessboard.
   */
  void performCastle(bool isShort, std::vector<std::vector<std::shared_ptr<Cell>>>& board);

  /*!
   \brief Checks if a specific cell is under attack.
   \details Determines whether a given cell is attacked by any opponent piece on the board.
   This is often used to check the validity of the king's moves.
   \param board A reference to the chessboard.
   \return `true` if the cell is under attack; otherwise, `false`.
   */
  bool IsCellUnderAttack(const std::vector<std::vector<std::shared_ptr<Cell>>>& board) const;

 private:
  bool hasMoved = false;  ///< Indicates whether the king has moved during the game.

 protected:
  Colour colour_;  ///< The color of the king (white or black).
};
