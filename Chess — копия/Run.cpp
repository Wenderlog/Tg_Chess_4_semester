//
//  Run.cpp
//  Chess
//
//  Created by Кирилл Грибанов on 25.12.2024.
//

#include <iostream>
#include <map>

#include "Game.h"
#include "Manager.h"
#include "Run.h"
#include "Table.h"

namespace {

/*!
 * \brief A namespace containing a mapping of `Table::TurnVerdict` values to corresponding messages.
 * \details This `namespace` provides a mapping between different game states (such as checkmate, stalemate, or invalid moves)
 * and their corresponding text messages. These messages are used to provide feedback to the player during the game.
 */

const std::map<Table::TurnVerdict, std::string> kShowingText = {
    
    /*!
     * \brief Message for when White is checkmated.
     * \details This message is displayed when White has been checkmated by Black.
     */
    
    {Table::TurnVerdict::white_mate, "Checkmate! White wins!\n"},

    /*!
     * \brief Message for when Black is checkmated.
     * \details This message is displayed when Black has been checkmated by White.
     */
    
    {Table::TurnVerdict::black_mate, "Checkmate! Black wins!\n"},

    /*!
     * \brief Message for when White is in stalemate.
     * \details This message is displayed when White is in a stalemate situation, resulting in a draw.
     */
    
    {Table::TurnVerdict::white_stalemate, "Stalemate! It's a draw.\n"},

    /*!
     * \brief Message for when Black is in stalemate.
     * \details This message is displayed when Black is in a stalemate situation, resulting in a draw.
     */
    {Table::TurnVerdict::black_stalemate, "Stalemate! It's a draw.\n"},

    /*!
     * \brief Message for an unnatural move.
     * \details This message is displayed when a move is not valid according to chess rules.
     */
    
    {Table::TurnVerdict::unnatural_move, "Unnatural move! Please try again.\n"},

    /*!
     * \brief Message for when a move results in a check.
     * \details This message is displayed when a move places the opponent's king in check.
     */
    
    {Table::TurnVerdict::check, "Check! Be careful, your king is in danger."},

    /*!
     * \brief Message for an invalid castling attempt.
     * \details This message is displayed when a player tries to castle in an invalid way.
     */
    {Table::TurnVerdict::invalid_rotate, "Invalid castling! The move is not allowed.\n"},

    /*!
     * \brief Message for pawn promotion.
     * \details This message is displayed when a pawn reaches the promotion rank and is ready to be promoted to another piece.
     */
    
    {Table::TurnVerdict::transformation, "Pawn promoted! Choose your new piece.\n"},

    /*!
     * \brief Message for when it is White's turn but they attempt to move out of turn.
     * \details This message is displayed when White tries to make a move when it is not their turn.
     */
    
    {Table::TurnVerdict::white_turn, "It's not your turn, white player!"},

    /*!
     * \brief Message for when it is Black's turn but they attempt to move out of turn.
     * \details This message is displayed when Black tries to make a move when it is not their turn.
     */
    
    {Table::TurnVerdict::black_turn, "It's not your turn, black player!"}
    
};

}

RunningGame::RunningGame() : chessTable_(), game_(chessTable_), manager_() {}

int RunningGame::Run() {
    
    try {
        game_.StartGame();

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

void RunningGame::DisplayBoardState() const {
    std::string boardState = chessTable_.GenerateBoardState();
    std::cout << "Current board state:\n" << boardState << std::endl;
}

std::string RunningGame::GetPlayerInput() const {
    std::string input;
    std::cout << "Enter your move (e.g., e2 e4 or O-O): ";
    std::getline(std::cin, input);
    return input;
}

bool RunningGame::HandlePlayerInput(const std::string& input) {
    if (input == "exit") {
        std::cout << "Game ended by the player." << std::endl;
        return false;
    }

    // Convert the move to coordinates
    auto coords = manager_.WordToCoord(chessTable_.getBoard(), input);
    std::cout << "From: (" << coords.first.col << ", " << coords.first.row
              << "), To: (" << coords.second.col << ", " << coords.second.row << ")" << std::endl;

    // Check for invalid coordinates
    if (coords.first.row == 8 || coords.second.row == 8) {
        std::cout << "Invalid move. Try again." << std::endl;
        return true;
    }

    // Validate and process the move
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

void RunningGame::CheckDrawConditions() {
    game_.CheckForRepetition();
    game_.CheckFor50MovesWithoutCapture();
}
