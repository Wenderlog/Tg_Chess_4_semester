//
//  Run.cpp
//  Chess
//
//  Created by Кирилл Грибанов on 25.12.2024.
//
#pragma once

#include <iostream>
#include <map>
#include <string>

#include "Game.h"
#include "Manager.h"
#include "Run.h"
#include "Table.h"

namespace {

/*!
 * \brief Contains mapping of `Table::TurnVerdict` values to corresponding messages.
 * \details This namespace provides a mapping between different game states (such as checkmate,
 * stalemate, or invalid moves) and their corresponding text messages, which are displayed to the player.
 */
const std::map<Table::TurnVerdict, std::string> kShowingText = {

    /*! \brief Message for when White is checkmated. */
    {Table::TurnVerdict::white_mate, "Checkmate! White wins!\n"},

    /*! \brief Message for when Black is checkmated. */
    {Table::TurnVerdict::black_mate, "Checkmate! Black wins!\n"},

    /*! \brief Message for when White is in stalemate. */
    {Table::TurnVerdict::white_stalemate, "Stalemate! It's a draw.\n"},

    /*! \brief Message for when Black is in stalemate. */
    {Table::TurnVerdict::black_stalemate, "Stalemate! It's a draw.\n"},

    /*! \brief Message for an unnatural move. */
    {Table::TurnVerdict::unnatural_move, "Unnatural move! Please try again.\n"},

    /*! \brief Message for a move resulting in a check. */
    {Table::TurnVerdict::check, "Check! Be careful, your king is in danger."},

    /*! \brief Message for an invalid castling attempt. */
    {Table::TurnVerdict::invalid_rotate, "Invalid castling! The move is not allowed.\n"},

    /*! \brief Message for pawn promotion. */
    {Table::TurnVerdict::transformation, "Pawn promoted! Choose your new piece.\n"},

    /*! \brief Message when it is White's turn, but they attempt to move out of turn. */
    {Table::TurnVerdict::white_turn, "It's not your turn, white player!"},

    /*! \brief Message when it is Black's turn, but they attempt to move out of turn. */
    {Table::TurnVerdict::black_turn, "It's not your turn, black player!"}
};

}

/*!
 * \brief Constructor for RunningGame class.
 * \details Initializes chess table, game, and manager.
 */
RunningGame::RunningGame() : chessTable_(), game_(chessTable_), manager_() {}

/*!
 * \brief Main game loop.
 * \return 0 if game ended normally, 1 if an exception occurred.
 */
int RunningGame::Run() {
    try {
        while (true) {
            DisplayBoardState();

            std::string input = GetPlayerInput();
            if (!HandlePlayerInput(input)) {
                break;
            }

            CheckDrawConditions();
        }

        game_.EndGame();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

/*! \brief Displays the current board state to the console. */
void RunningGame::DisplayBoardState() const {
    std::string boardState = chessTable_.GenerateBoardState();
    std::cout << "Current board state:\n" << boardState << std::endl;
}

/*! \brief Returns the current board state as a string. */
std::string RunningGame::GetBoardState() const {
    return chessTable_.GenerateBoardState();
}

/*! \brief Reads player input from console. */
std::string RunningGame::GetPlayerInput() const {
    std::string input;
    std::cout << "Enter your move (e.g., e2 e4 or O-O): ";
    std::getline(std::cin, input);
    return input;
}

/*!
 * \brief Handles a player's input.
 * \param input The input string representing the move.
 * \return false if the game should end, true otherwise.
 */
bool RunningGame::HandlePlayerInput(const std::string& input) {
    if (input == "exit") {
        std::cout << "Game ended by the player." << std::endl;
        return false;
    }

    auto coords = manager_.WordToCoord(chessTable_.getBoard(), input);
    std::cout << "From: (" << coords.first.col << ", " << coords.first.row
              << "), To: (" << coords.second.col << ", " << coords.second.row << ")" << std::endl;

    if (coords.first.row == 8 || coords.second.row == 8) {
        std::cout << "Invalid move. Try again." << std::endl;
        return true;
    }

    auto turnVerdict = chessTable_.CheckTurn(coords.first, coords.second);
    if (turnVerdict == Table::TurnVerdict::correct) {
        chessTable_.DoTurn(coords.first, coords.second);
    } else {
        std::cout << kShowingText.at(turnVerdict) << std::endl;
        if (turnVerdict == Table::TurnVerdict::white_mate || turnVerdict == Table::TurnVerdict::black_mate ||
            turnVerdict == Table::TurnVerdict::white_stalemate || turnVerdict == Table::TurnVerdict::black_stalemate) {
            return false;
        }
    }

    return true;
}

/*! \brief Checks draw conditions for the game. */
void RunningGame::CheckDrawConditions() {
    game_.CheckForRepetition();
    game_.CheckFor50MovesWithoutCapture();
}

/*!
 * \brief Handles a move given a string and player color.
 * \param move Move as a string.
 * \param color Player color ("White" or "Black").
 * \return true if the move is valid and executed, false otherwise.
 */
bool RunningGame::HandleMove(const std::string& move, const std::string& color) {
    auto coords = manager_.WordToCoord(chessTable_.getBoard(), move);
    if (coords.first.row == 8 || coords.second.row == 8) {
        return false;
    }

    Colour current_turn = chessTable_.GetCurrentTurn();

    if ((color == "White" && current_turn != Colour::WHITE) ||
        (color == "Black" && current_turn != Colour::BLACK)) {
        return false;
    }

    auto turnVerdict = chessTable_.CheckTurn(coords.first, coords.second);
    if (turnVerdict == Table::TurnVerdict::correct) {
        chessTable_.DoTurn(coords.first, coords.second);
        return true;
    }

    std::cerr << "Invalid move!" << std::endl;
    return false;
}
