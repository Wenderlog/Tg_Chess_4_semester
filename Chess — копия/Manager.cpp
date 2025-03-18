//  Manager.cpp
//  Chess
//
//  Created by Кирилл Грибанов on 08.12.2024.
//

#include "Manager.h"

Coord Manager::ConvertToCoord(const std::string& square) {
    int col = square[0] - 'a';
    int row = 8 - (square[1] - '0');
    return {row, col};
}

bool Manager::IsValidCoord(const Coord& coord) {
    return coord.row >= 0 && coord.row < 8 && coord.col >= 0 && coord.col < 8;
}

std::pair<Coord, Coord> Manager::HandleWhiteCastle(const std::vector<std::vector<std::shared_ptr<Cell>>>& board, const std::string& message) {
    auto king = std::dynamic_pointer_cast<KingCell>(board[0][4]);
    if (king && king->canCastle(true, board)) {
        return {{0, 4}, {0, 6}};
    } else if (king && king->canCastle(false, board)) {
        return {{0, 4}, {0, 2}};
    }
    return {};
}

std::pair<Coord, Coord> Manager::HandleBlackCastle(const std::vector<std::vector<std::shared_ptr<Cell>>>& board, const std::string& message) {
    auto king = std::dynamic_pointer_cast<KingCell>(board[7][4]);
    if (king && king->canCastle(true, board)) {
        return {{7, 4}, {7, 6}};
    } else if (king && king->canCastle(false, board)) {
        return {{7, 4}, {7, 2}};
    }
    return {};
}

std::pair<Coord, Coord> Manager::WordToCoord(const std::vector<std::vector<std::shared_ptr<Cell>>>& board, const std::string& message) {
    if (message == "O-O" || message == "O-O-O") {
        return HandleWhiteCastle(board, message);
    }

    if (message == "o-o" || message == "o-o-o") {
        return HandleBlackCastle(board, message);
    }

    std::istringstream stream(message);
    std::string fromSquare, toSquare;
    stream >> fromSquare >> toSquare;

    auto fromCoord = ConvertToCoord(fromSquare);
    auto toCoord = ConvertToCoord(toSquare);

    if (!IsValidCoord(fromCoord) || !IsValidCoord(toCoord)) {
        std::cout << "Coords aren't valid!" << std::endl;
        return {};
    }

    auto fromCell = board[fromCoord.row][fromCoord.col];
    auto toCell = board[toCoord.row][toCoord.col];
    auto reservedSteps = fromCell->getReservedSteps();

    if (std::find(reservedSteps.begin(), reservedSteps.end(), toCoord) != reservedSteps.end()) {
        return {fromCoord, toCoord};
    }

    return {};
}
