//
// Created by Кирилл Грибанов  on 01/04/2025.
//

#pragma once

#include <httplib.h>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Table.h"
#include "/Users/wenderlender/Desktop/Chess/Server_Manager.h"

/*!
 * \class ChessServer
 * \brief Represents a chess server that handles game logic and communication.
 * \details This class sets up a server using the `httplib` library to manage requests related to chess gameplay.
 * It supports the following operations:
 *    - Player authentication (`/auth` endpoint)
 *    - Player moves (`/move` endpoint)
 *    - Game status (`/status` endpoint)
 *    - Board state (`/board` endpoint)
 *
 * The server uses a `Table` object to manage the chessboard and a `Manager` object to handle moves and validate actions.
 * It ensures thread safety using a `std::mutex` to protect shared resources.
 */
class ChessServer {
public:
    /*!
     * \brief Default constructor for the ChessServer class.
     * \details Initializes the chess server, setting up necessary objects for handling the game logic,
     *          but does not start the server until the `runServer` method is called.
     */
    ChessServer()
        : game_mutex()
        , id_generator(1)
        , manager_("host=localhost port=5433 dbname=mydb user=myuser password=mypassword")
        , running_game_()
        , database_("host=localhost port=5433 dbname=mydb user=myuser password=mypassword")
        , table()
        , player_map()
        , game_players()
        , pending_game_id(std::nullopt)
    {}

    /*!
     * \brief Handles player authentication requests.
     * \param req Incoming HTTP request.
     * \param res HTTP response to be sent back to the client.
     */
    void doAuth(const httplib::Request &req, httplib::Response &res);

    /*!
     * \brief Starts the chess server and begins listening for requests.
     * \details This method sets up the server to handle incoming HTTP requests related to the chess game.
     *          It includes routes for player authentication, moves, game status, and board state.
     */
    void runServer();

private:
    std::mutex game_mutex;  ///< Mutex for thread-safe access to shared resources.

    idGenerator id_generator{1};  ///< Unique ID generator (starts at 1).
    Games_Manager manager_{"host=localhost port=5433 dbname=mydb user=myuser password=mypassword"};  ///< Manager for all active games.
    RunningGame running_game_;  ///< Object managing a running chess game.
    DataBase database_{"host=localhost port=5433 dbname=mydb user=myuser password=mypassword"};  ///< Connection to the database.
    Table table;  ///< Chessboard and game logic handler.
    std::unordered_map<int, std::string> player_map;  ///< Mapping: player_id → color ("White" / "Black").
    std::unordered_map<int, std::vector<int>> game_players;  ///< Mapping: game_id → list of player IDs.
    std::optional<int> pending_game_id;  ///< ID of the game waiting for players to join.
};
