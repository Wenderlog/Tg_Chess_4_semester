//
//  King_Cell.cpp
//  Chess
//
//  Created by Кирилл Грибанов  on 06.12.2024.
//

#include "King_Cell.h"

KingCell::KingCell(Coord coord, Colour colour) : Cell(coord), colour_(colour) {
}

Colour KingCell::getColour() const {
  return colour_;
}

std::unordered_set<Coord> KingCell::getReservedSteps() const {
  std::unordered_set<Coord> moves;
  moves.reserve(8);

  for (int drow = -1; drow < 2; ++drow) {
    for (int dcol = -1; dcol < 2; ++dcol) {
      if (drow == 0 && dcol == 0)
        continue;
      int new_row = coord_.row + drow;
      int new_col = coord_.col + dcol;
      if (new_row < 8 && new_row >= 0 && new_col < 8 && new_col >= 0) {
        moves.insert({new_row, new_col});
      }
    }
  }
  return moves;
}

std::unordered_set<Coord> KingCell::getHits() const {
  return getReservedSteps();
}

std::string_view KingCell::Name() const {
  return KingName;
}

char KingCell::getSymbol() const {
  return (colour_ == Colour::WHITE) ? 'K' : 'k';
}

bool KingCell::IsCellUnderAttack(const std::vector<std::vector<std::shared_ptr<Cell>>>& board) const {

  Colour opponentColour = (colour_ == Colour::WHITE) ? Colour::BLACK : Colour::WHITE;

  for (const auto& row : board) {
    for (const auto& cell : row) {
      if (cell->getColour() == opponentColour) {
        auto hits = cell->getHits();
        if (std::find(hits.begin(), hits.end(), coord_) != hits.end()) {
          return true;
        }
      }
    }
  }
  return false;
}

bool KingCell::canCastle(bool isShort, const std::vector<std::vector<std::shared_ptr<Cell>>>& board) const {
  if (hasMoved)
    return false;  // перенести в Table

  int row = coord_.row;
  int col = coord_.col;

  int rookCol = isShort ? 7 : 0;
  int direction = isShort ? 1 : -1;

  auto rook = std::dynamic_pointer_cast<RookCell>(board[row][rookCol]);
  if (!rook || rook->getColour() != colour_ || rook->hasMoved()) {
    return false;
  }

  for (int c = col + direction; c != rookCol; c += direction) {
    if (board[row][c]->Name() != EmptyName)
      return false;
  }

  for (int c = col; c != col + 2 * direction; c += direction) {
    if (KingCell::IsCellUnderAttack(board)) {
      return false;
    }
  }

  return true;
}

void KingCell::performCastle(bool isShort, std::vector<std::vector<std::shared_ptr<Cell>>>& board) {
  int row = coord_.row;
  int col = coord_.col;

  int rookCol = isShort ? 7 : 0;
  int direction = isShort ? 1 : -1;

  board[row][col + 2 * direction] = board[row][col];
  board[row][col] = std::make_shared<EmptyCell>(Coord{row, col});
  coord_.col += 2 * direction;

  board[row][col + direction] = board[row][rookCol];
  board[row][rookCol] = std::make_shared<EmptyCell>(Coord{row, rookCol});
}
