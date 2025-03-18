//
//  Run.h
//  Chess
//
//  Created by Кирилл Грибанов on 25.12.2024.
//

#pragma once

#include <string>

/*!
 * \class RunningGame
 * \brief Manages the execution of a chess game.
 * \details The `RunningGame` class encapsulates the main game loop, board display,
 * player input handling, and game termination conditions. It integrates `Table`,
 * `Game`, and `Manager` components to manage the chess game.
 */

class RunningGame {
public:

    /*!
     * \brief Constructs a `RunningGame` object.
     * \details Initializes the chess table, game logic, and move manager.
     */
    
    RunningGame();

    /*!
     * \brief Default destructor for the `RunningGame` class.
     */
    
    ~RunningGame() = default;

    /*!
     * \brief Starts and manages the main game loop.
     * \details Handles player input, game state updates, and checks for endgame conditions.
     * \return int Exit code: 0 for successful execution, 1 for errors.
     */
    
    int Run();

private:

    /*!
     * \brief Displays the current state of the chessboard.
     * \details Uses the `Table` object to generate and print a visual representation of the board.
     */
    
    void DisplayBoardState() const;

    /*!
     * \brief Prompts the player for input.
     * \return std::string The player's input string, representing a move or command.
     */
    
    std::string GetPlayerInput() const;

    /*!
     * \brief Processes the player's input.
     * \details Validates and executes moves or displays error messages for invalid inputs.
     * \param input The player's input string.
     * \return bool `false` if the game should end, otherwise `true`.
     */
    
    bool HandlePlayerInput(const std::string& input);

    /*!
     * \brief Checks for draw conditions.
     * \details Evaluates the game state for draw scenarios such as threefold repetition
     * or 50 moves without a capture.
     */
    
    void CheckDrawConditions();

    Table chessTable_; ///< Represents the chessboard and its state.
    Game game_;        ///< Manages game logic, including turn management and endgame checks.
    Manager manager_;  ///< Handles move parsing and coordinate translation.
};
