//
//  Game.cpp
//  Chess
//
//  Created by Кирилл Грибанов  on 11.12.2024.
//

#include "Game.h"

void Game::CheckFor50MovesWithoutCapture() {
    if (movesWithoutCapture >= 50) {
        std::cout << "50 moves without capture detected: draw!" << std::endl;
        EndGame();
    }
}

void Game::MakeMove(Coord from, Coord to) {

    auto fromCell = chessTable.GetCell(from.row, from.col);
    auto toCell = chessTable.GetCell(to.row, to.col);


    if (chessTable.CheckColourToAtack(from, to, lastMoveWasCapture)) {
        lastMoveWasCapture = true;
        movesWithoutCapture = 0;
    } else {
        lastMoveWasCapture = false;
        movesWithoutCapture++;
    }

    chessTable.DoTurn(from, to);

    CheckFor50MovesWithoutCapture();
}

void Game::CheckForRepetition() {
    static std::deque<std::string> recentStates;

    std::string boardState = chessTable.GenerateBoardState();
    recentStates.push_back(boardState);

    if (recentStates.size() > 3) {
        recentStates.pop_front();
    }

    if (recentStates.size() == 3 &&
        recentStates[0] == recentStates[1] &&
        recentStates[1] == recentStates[2]) {
        std::cout << "Threefold repetition detected: draw!" << std::endl;
        EndGame();
    }
}

void Game::EndGame() {
    std::cout << "Game over" << std::endl;
}

void Game::StartGame() {
    std::cout << "Starting game..." << std::endl;
    chessTable = Table();
    movesWithoutCapture = 0;
    lastMoveWasCapture = false;
    std::cout << "Initial Board State:\n";
    auto picture = chessTable.GetPicture();
    for (const auto& row : picture) {
        std::cout << row << std::endl;
    }
}

Colour Game::GetCurrentTurnColour() const {
    std::string boardState = chessTable.GenerateBoardState();
    char turnSymbol = boardState.back();

    if (turnSymbol == 'w') {
        return Colour::WHITE;
    } else if (turnSymbol == 'b') {
        return Colour::BLACK;
    } else {
        throw std::runtime_error("Unknown turn symbol in board state");
    }
}
