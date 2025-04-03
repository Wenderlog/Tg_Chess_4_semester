//
// Created by Кирилл Грибанов  on 01/04/2025.
//
#pragma once

#include <httplib.h>
#include <mutex>
#include <string>
#include <map>
#include "/Users/wenderlender/Desktop/Chess/Table.h"
#include "/Users/wenderlender/Desktop/Chess/Manager.h"

/*!
 \class ChessServer
 \brief Represents a chess server that handles game logic and communication.
 \details This class sets up a server using the `httplib` library to manage requests related to chess gameplay.
 It supports the following operations:
    - Player authentication (`/auth` endpoint)
    - Player moves (`/move` endpoint)
    - Game status (`/status` endpoint)
    - Board state (`/board` endpoint)

 The server uses a `Table` object to manage the chessboard and a `Manager` object to handle moves and validate actions.
 It ensures thread safety using a `std::mutex` to protect shared resources.
 */
class ChessServer {
public:
    /*!
     \brief Default constructor for the ChessServer class.
     \details Initializes the chess server, setting up necessary objects for handling the game logic,
              but does not start the server until the `runServer` method is called.
     */
    ChessServer() = default;

    /*!
     \brief Starts the chess server and begins listening for requests.
     \details This method sets up the server to handle incoming HTTP requests related to the chess game.
              It includes routes for player authentication, moves, game status, and board state.
     */
    void runServer();

private:
    std::mutex game_mutex;  ///< Mutex for thread safety to protect shared resources.
    Table table;            ///< The chessboard and game logic handler.
    Manager manager_;       ///< The manager responsible for move validation and logic.
    std::map<std::string, std::string> player_map;  ///< Map storing player IDs and their associated colors.
    std::string white_player_id;  ///< Player ID for the white player.
    std::string black_player_id;  ///< Player ID for the black player.
};