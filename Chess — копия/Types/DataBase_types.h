//
// Created by Кирилл Грибанов  on 21/05/2025.
//

#pragma once
#include <string>

/*! \brief
 *   Represents a chess move with starting and ending cells.
 */
struct Turn {
    std::string from;  ///< The coordinate of the cell from which the move is made (e.g., "e2").
    std::string to;    ///< The coordinate of the cell to which the move is made (e.g., "e4").
};

/*! \brief
 *   Stores information about a move in the context of a game.
 *
 *   Contains the player ID, the game ID, and the move itself.
 */
struct TurnInfo {
    int player_id;  ///< Unique identifier of the player making the move.
    int game_id;    ///< Unique identifier of the game in which the move occurs.
    Turn turn;      ///< The actual move performed by the player.
};