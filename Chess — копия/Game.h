//  Game.h
//  Chess
//
//  Created by Кирилл Грибанов on 11.12.2024.
//

#pragma once

#include "Table.h"

/*!
 * \class Game
 * \brief A class representing the game logic and rules of a chess game.
 * \details This class is responsible for managing the state of the game, including starting and ending the game,
 * making moves, checking for draw conditions, and keeping track of the game status. It interacts with the `Table` class,
 * which represents the chessboard and the pieces on it.
 */

class Game {
public:
    /*!
     * \brief Constructor for the Game class.
     * \details Initializes a new instance of the Game class with a reference to the `Table` class that represents the chessboard.
     * \param table A reference to the `Table` class that holds the chessboard and pieces.
     */
    
    Game(Table& table) : chessTable(table) {}

    /*!
     * \brief Checks for the repetition of moves in the game.
     * \details This method checks whether the current game position has occurred three times, which could lead to a draw by repetition.
     */
    
    void CheckForRepetition();

    /*!
     * \brief Ends the game.
     * \details This method ends the game, marking it as over. It could be triggered by checkmate, stalemate, or any other game-ending condition.
     */
    
    void EndGame();

    /*!
     * \brief Starts the game.
     * \details This method initializes the game by setting up the board and pieces, and setting the initial game state.
     */
    
    void StartGame();

    /*!
     * \brief Makes a move on the board.
     * \details This method moves a piece from one position to another on the chessboard. It checks whether the move is valid,
     * updates the game state, and switches the turn.
     * \param from The coordinates of the starting square.
     * \param to The coordinates of the destination square.
     */
    
    void MakeMove(Coord from, Coord to);

    /*!
     * \brief Checks if 50 moves have passed without a capture or pawn move.
     * \details This method checks if the 50-move rule has been satisfied, where no capture or pawn move has occurred
     * in the last 50 moves, which could lead to a draw.
     */
    
    void CheckFor50MovesWithoutCapture();

    /*!
     * \brief Gets the color of the player whose turn it is.
     * \details This method returns the color (either white or black) of the player who is currently making the move.
     * \return Colour The color of the player whose turn it is.
     */
    
    Colour GetCurrentTurnColour() const;

private:
    Table& chessTable; ///< Reference to the `Table` class representing the chessboard.
    int movesWithoutCapture = 0; ///< Tracks the number of moves without a capture.
    bool lastMoveWasCapture = false; ///< Tracks whether the last move was a capture.
};
