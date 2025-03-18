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

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <string_view>
#include <unordered_set>
/*!\namespace
 *\brief Allows the compiler to evaluate the value of an expression immediately, making programs more optimized.
 */

enum class Colour { BLACK, WHITE };

/*! \brief Created colored squares in chess
 */

struct Coord {
  int row = 8;
  int col = 8;
  bool operator==(Coord other) const;
};

namespace std {
template <>
class hash<Coord> {
 public:
  size_t operator()(Coord coord) const;
};
} 

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
  // virtual Cell* Copy() const = 0; ///< Created to copy a cell but so far not used anywhere in the code
  virtual char getSymbol() const = 0;

 protected:
  Coord coord_;  ///< The coordinates are protected so they should be visible to other figures but not changeable.
};
