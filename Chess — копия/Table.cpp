//
//  Table.cpp
//  Chess
//
//  Created by Кирилл Грибанов  on 10.12.2024.
//

#include "Table.h"

auto& Table::operator[](size_t i) {
  return body_[i];
}

const auto& Table::operator[](size_t i) const {
  return body_[i];
}

Table::Table()
    : body_(8, std::vector<std::shared_ptr<Cell>>(8))
    , w_king_moves_(false)
    , b_king_moves_(false)
    , wl_rook_moves_(false)
    , wr_rook_moves_(false)
    , bl_rook_moves_(false)
    , br_rook_moves_(false)
    , last_move_is_w_pawn_(false)
    , last_move_is_b_pawn_(false)
    , coord_w_pawn_({0, 0})
    , coord_b_pawn_({0, 0})
    ,

    currentTurn(Colour::WHITE) {

  body_[0][4].reset(new KingCell({0, 4}, Colour::WHITE));
  body_[0][3].reset(new QueenCell({0, 3}, Colour::WHITE));
  body_[0][2].reset(new BishopCell({0, 2}, Colour::WHITE));
  body_[0][1].reset(new KnightCell({0, 1}, Colour::WHITE));
  body_[0][0].reset(new RookCell({0, 0}, Colour::WHITE));
  body_[0][7].reset(new RookCell({0, 7}, Colour::WHITE));
  body_[0][6].reset(new KnightCell({0, 6}, Colour::WHITE));
  body_[0][5].reset(new BishopCell({0, 5}, Colour::WHITE));
  for (int i = 0; i < 8; ++i) {
    body_[1][i].reset(new PawnCell({1, i}, Colour::WHITE));
  }

  body_[7][4].reset(new KingCell({7, 4}, Colour::BLACK));
  body_[7][3].reset(new QueenCell({7, 3}, Colour::BLACK));
  body_[7][2].reset(new BishopCell({7, 2}, Colour::BLACK));
  body_[7][1].reset(new KnightCell({7, 1}, Colour::BLACK));
  body_[7][0].reset(new RookCell({7, 0}, Colour::BLACK));
  body_[7][7].reset(new RookCell({7, 7}, Colour::BLACK));
  body_[7][6].reset(new KnightCell({7, 6}, Colour::BLACK));
  body_[7][5].reset(new BishopCell({7, 5}, Colour::BLACK));
  for (int i = 0; i < 8; ++i) {
    body_[6][i].reset(new PawnCell({6, i}, Colour::BLACK));
  }
  for (int i = 2; i < 6; ++i) {
    for (int j = 0; j < 8; ++j) {
      body_[i][j].reset(new EmptyCell({i, j}));
    }
  }
}

bool Table::CheckColourToAtack(Coord from, Coord to, bool parametr) const {
  parametr = false;
  if (body_[from.row][from.col]->getColour() != body_[to.row][to.col]->getColour()) {
    parametr = true;
  }
  return parametr;
}

void Table::DoTurn(Coord from, Coord to) {
  if (CheckTurn(from, to) == TurnVerdict::correct) {
    body_[to.row][to.col] = body_[from.row][from.col];
    body_[from.row][from.col].reset(new EmptyCell({from.row, from.col}));

    auto& movedPiece = body_[to.row][to.col];
    if (movedPiece && movedPiece->Name() == PawnName) {
      if (movedPiece->getColour() == Colour::WHITE && to.row == 7) {
        PromotePawn(to, 'q');
      } else if (movedPiece->getColour() == Colour::BLACK && to.row == 0) {
        PromotePawn(to, 'Q');
      }
    }
    if (movedPiece && movedPiece->Name() == KingName) {
      if (movedPiece->getColour() == Colour::WHITE) {
        w_king_moves_ = true;
      } else {
        b_king_moves_ = true;
      }
    }

    if (movedPiece && movedPiece->Name() == RookName) {
      if (from.row == 0 && from.col == 0) {
        wl_rook_moves_ = true;
      }
      if (from.row == 7 && from.col == 0) {
        bl_rook_moves_ = true;
      }
      if (from.row == 0 && from.col == 7) {
        wr_rook_moves_ = true;
      }
      if (from.row == 7 && from.col == 7) {
        br_rook_moves_ = true;
      }
    }
    currentTurn = (currentTurn == Colour::WHITE) ? Colour::BLACK : Colour::WHITE;
  }
}

std::string Table::GenerateBoardState() const {
  std::string boardState = "";

  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      boardState += body_[row][col]->getSymbol();
      if (col < 7) {
        boardState += " ";
      }
    }
    boardState += "\n";
  }

  return boardState;
}
std::vector<std::string> Table::GetPicture() const {
  std::vector<std::string> draw(8);
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      draw[row] += body_[row][col]->getSymbol();
      draw[row] += ' ';
    }
  }
  return draw;
}

Colour Table::GetCurrentTurnColour() const {
  std::string boardState = GenerateBoardState();
  char turnSymbol = boardState.back();

  if (turnSymbol == 'w') {
    return Colour::WHITE;
  } else if (turnSymbol == 'b') {
    return Colour::BLACK;
  } else {
    throw std::runtime_error("Unknown turn symbol in board state");
  }
}

bool Table::CheckAttack(Coord from, Coord to) const {

  if (from.row < 0 || from.row >= 8 || from.col < 0 || from.col >= 8 || to.row < 0 || to.row >= 8 || to.col < 0 ||
      to.col >= 8) {
    return false;
  }
  auto fromCell = body_[from.row][from.col];
  auto toCell = body_[to.row][to.col];

  if (fromCell->Name() == PawnName) {
    Colour pawnColour = fromCell->getColour();
    int direction = (pawnColour == Colour::WHITE) ? 1 : -1;

    // if ((last_move_is_w_pawn_ || last_move_is_b_pawn_) && coord_w_pawn_ == Coord(to.row - direction, to.col)) {
    //   if (toCell == nullptr && abs(from.col - to.col) == 1 && to.row == from.row + direction) {
    //     return true;
    //   }
    // }

    if (toCell->getColour() != fromCell->getColour() && abs(from.col - to.col) == 1 && to.row == from.row + direction) {
      return true;
    }

    return false;
  }

  if (fromCell->Name() == RookName) {

    int rowDirection = (to.row > from.row) ? 1 : (to.row < from.row) ? -1 : 0;
    int colDirection = (to.col > from.col) ? 1 : (to.col < from.col) ? -1 : 0;

    Coord current = from;
    current.row += rowDirection;
    current.col += colDirection;

    while (current != to) {
      if (current.row < 0 || current.row >= 8 || current.col < 0 || current.col >= 8) {
        return false;
      }

      if (body_[current.row][current.col] != nullptr) {
        return false;
      }

      current.row += rowDirection;
      current.col += colDirection;
    }

    return true;
  }

  if (fromCell->Name() == BishopName) {

    int rowDirection = (to.row > from.row) ? 1 : (to.row < from.row) ? -1 : 0;
    int colDirection = (to.col > from.col) ? 1 : (to.col < from.col) ? -1 : 0;

    if (abs(to.row - from.row) != abs(to.col - from.col)) {
      return false;
    }

    Coord current = from;
    current.row += rowDirection;
    current.col += colDirection;

    while (current != to) {
      if (current.row < 0 || current.row >= 8 || current.col < 0 || current.col >= 8) {
        return false;
      }

      if (body_[current.row][current.col] != nullptr) {
        return false;
      }

      current.row += rowDirection;
      current.col += colDirection;
    }

    return true;
  }

  if (fromCell->Name() == QueenName) {

    int rowDirection = (to.row > from.row) ? 1 : (to.row < from.row) ? -1 : 0;
    int colDirection = (to.col > from.col) ? 1 : (to.col < from.col) ? -1 : 0;

    Coord current = from;
    current.row += rowDirection;
    current.col += colDirection;

    while (current != to) {
      if (current.row < 0 || current.row >= 8 || current.col < 0 || current.col >= 8) {
        return false;
      }

      if (body_[current.row][current.col] != nullptr) {
        return false;
      }

      current.row += rowDirection;
      current.col += colDirection;
    }

    rowDirection = (to.row > from.row) ? 1 : (to.row < from.row) ? -1 : 0;
    colDirection = (to.col > from.col) ? 1 : (to.col < from.col) ? -1 : 0;

    current = from;
    current.row += rowDirection;
    current.col += colDirection;

    while (current != to) {
      if (current.row < 0 || current.row >= 8 || current.col < 0 || current.col >= 8) {
        return false;
      }

      if (body_[current.row][current.col] != nullptr) {
        return false;
      }

      current.row += rowDirection;
      current.col += colDirection;
    }

    return true;
  }

  if (fromCell->Name() == KnightName) {
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);

    if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
      return true;
    }

    return false;
  }

  if (fromCell->Name() == KingName) {
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);

    if (rowDiff <= 1 && colDiff <= 1) {
      return true;
    }

    return false;
  }

  return false;
}

void Table::DoAttack(Coord from, Coord to) {
  if (CheckAttack(from, to)) {
    if (body_[from.row][from.col]->Name() == KingName) {
      if (body_[from.row][from.col]->getColour() == Colour::WHITE) {
        w_king_moves_ = true;
      } else {
        b_king_moves_ = true;
      }
    }
    if (body_[from.row][from.col]->Name() == RookName) {
      if (body_[from.row][from.col] == body_[0][0]) {
        wl_rook_moves_ = true;
      } else if (body_[from.row][from.col] == body_[0][7]) {
        bl_rook_moves_ = true;
      } else if (body_[from.row][from.col] == body_[7][0]) {
        bl_rook_moves_ = true;
      } else if (body_[from.row][from.col] == body_[7][7]) {
        br_rook_moves_ = true;
      }
    }
    std::swap(body_[from.row][from.col], body_[to.row][to.col]);
    body_[from.row][from.col].reset(new EmptyCell({from.row, from.col}));
  }
}

Table::TurnVerdict Table::CheckTurn(Coord from, Coord to) const {
  auto fromCell = body_[from.row][from.col];
  auto toCell = body_[to.row][to.col];

  if (fromCell->Name() == EmptyName) {
    return TurnVerdict::unnatural_move;
  }

  if (fromCell->getColour() != currentTurn) {
    return TurnVerdict::unnatural_move;
  }

  if (toCell->Name() != EmptyName && fromCell->getColour() == toCell->getColour()) {
    return TurnVerdict::unnatural_move;
  }

  std::cout << "done1\n";

  if (toCell->Name() != EmptyName && !CheckAttack(from, to)) {
    return TurnVerdict::unnatural_move;
  }

  if (!CheckMove(from, to)) {
    return TurnVerdict::unnatural_move;
  }

  if (!CheckChessValid(from, to)) {
    return TurnVerdict::unnatural_move;
  }

  return TurnVerdict::correct;
}

bool Table::CheckMove(Coord from, Coord to) const {
  auto reserved_steps = body_[from.row][from.col]->getReservedSteps();
  if (reserved_steps.find(to) != reserved_steps.end()) {
    return true;
  }
  return false;
}

void Table::PromotePawn(Coord position, char promotionType)  // переписать char-> enum class
{
  auto& cell = body_[position.row][position.col];

  if (!cell || cell->Name() != PawnName) {
    throw std::invalid_argument("No pawn at the specified position to promote.");
  }

  if ((cell->getColour() == Colour::WHITE && position.row != 7) ||
      (cell->getColour() == Colour::BLACK && position.row != 0)) {
    throw std::invalid_argument("Pawn is not in the promotion row.");
  }

  Colour colour = cell->getColour();
  switch (promotionType) {
    case 'Q':
    case 'q':
      cell.reset(new QueenCell(position, colour));
      break;
    case 'R':
    case 'r':
      cell.reset(new RookCell(position, colour));
      break;
    case 'B':
    case 'b':
      cell.reset(new BishopCell(position, colour));
      break;
    case 'N':
    case 'n':
      cell.reset(new KnightCell(position, colour));
      break;
    default:
      throw std::invalid_argument("Invalid promotion type.");
  }

  std::cout << "Pawn at (" << position.row << ", " << position.col << ") promoted to " << promotionType << ".\n";
}

std::shared_ptr<Cell>& Table::GetCell(int row, int col) {
  return body_[row][col];
}

const std::shared_ptr<Cell>& Table::GetCell(int row, int col) const {
  return body_[row][col];
}

bool Table::CheckChessValid(Coord from, Coord to) const {
  std::swap(body_[from.row][from.col], body_[to.row][to.col]);
  const KingCell* king_ptr = nullptr;
  if (currentTurn == Colour::WHITE) {
    Coord white_king = WhiteKing();
    king_ptr = dynamic_cast<const KingCell*>(body_[white_king.row][white_king.col].get());
  } else {
    Coord black_king = BlackKing();
    king_ptr = dynamic_cast<const KingCell*>(body_[black_king.row][black_king.col].get());
  }
  if (king_ptr == nullptr) {
    std::swap(body_[from.row][from.col], body_[to.row][to.col]);
    return false;
  }
  if (king_ptr->IsCellUnderAttack(body_)) {
    std::swap(body_[from.row][from.col], body_[to.row][to.col]);
    return false;
  }
  std::swap(body_[from.row][from.col], body_[to.row][to.col]);
  return true;
}

Coord Table::WhiteKing() const {
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      auto& cell = body_[row][col];
      if (cell && cell->Name() == KingName && cell->getColour() == Colour::WHITE) {
        return {row, col};
      }
    }
  }
  return {};
}

Coord Table::BlackKing() const {
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      auto& cell = body_[row][col];
      if (cell && cell->Name() == KingName && cell->getColour() == Colour::BLACK) {
        return {row, col};
      }
    }
  }
  return {};
}
