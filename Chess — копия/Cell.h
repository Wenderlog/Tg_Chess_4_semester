//
//  Cell_File.h
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#pragma once

/*! \brief
 *   Inherits from the Cell class and implements abstract methods to obtain
 cell color, available moves and cell name.
 */

#include "Types/Game_types.h"
#include <string_view>
#include <unordered_set>

/*! \brief Supports the basic functions of a regular cell in chess
 */

class Cell {
 public:
  virtual Colour getColour() const = 0;  ///< The method was created to obtain chess board squares
  Cell(Coord coord);  ///< The Cell constructor and deconstructor have been created with parameters of the Coord type
                      ///< for further implementation of board coordinates
  virtual ~Cell() = default;
  virtual std::unordered_set<Coord> getHits() const = 0;  ///< Displays a vector of possible impacts
  virtual std::unordered_set<Coord> getReservedSteps() const = 0; ///< The method calculates possible moves of the figure
  virtual std::string_view Name() const = 0;  ///< Created for assigning a name to a figure
  virtual char getSymbol() const = 0;

 protected:
  Coord coord_;  ///< The coordinates are protected so they should be visible to other figures but not changeable.
};
