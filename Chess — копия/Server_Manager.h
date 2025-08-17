//
// Created by Кирилл Грибанов  on 05/04/2025.
//

#include <atomic>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <condition_variable>
#include <map>
#include <string>
#include <thread>

#include "DataBase.h"
#include "Game.h"
#include "Run.h"

/*!
 * \class idGenerator
 * \brief Generates unique integer IDs for games or players.
 */
class idGenerator {
public:
    /*!
     * \brief Constructor initializing the ID generator.
     * \param id Starting ID value.
     */
    explicit idGenerator(int id);

    /*!
     * \brief Returns the next unique ID.
     * \return Next integer ID.
     */
    int NextID();

private:
    std::atomic<int> id_;  ///< Atomic counter for generating unique IDs.
};

/*!
 * \class Games_Manager
 * \brief Manages active chess games and handles interaction with the database.
 */
class Games_Manager {
public:
    /*!
     * \brief Constructor with database connection string.
     * \param db_conn_str PostgreSQL connection string.
     */
    explicit Games_Manager(const std::string& db_conn_str)
        : id_generator_(1),                 // Start ID generator from 1
          database_(db_conn_str),           // Connect to the database
          game_started_(false),             // Game initially not started
          table_(),                         // Initialize chess board
          start_game_(table_),              // Game depends on table
          running_game_()                   // Default running game
    {}

    /*!
     * \brief Generates a new game and returns its unique ID.
     * \return New unique game ID.
     */
    int GenerateGames();

    /*!
     * \brief Returns a reference to the main chess table.
     * \return Reference to the Table object.
     */
    Table& GetTable() { return table_; }

    /*!
     * \brief Returns a constant reference to the main chess table.
     * \return Constant reference to the Table object.
     */
    const Table& GetTable() const { return table_; }

    /*!
     * \brief Retrieves a running game by its ID.
     * \param id_game Game ID.
     * \return Shared pointer to the RunningGame object, or nullptr if not found.
     */
    std::shared_ptr<RunningGame> GetGame(int id_game);

    /*!
     * \brief Returns the current board state for a given game.
     * \param id_game Game ID.
     * \return Board state as a string.
     */
    std::string GetBoardState(int id_game);

private:
    idGenerator id_generator_;                             ///< Unique ID generator.
    DataBase database_;                                    ///< Database interface.

    std::atomic<bool> game_started_;                       ///< Flag indicating if a game has started.
    Table table_;                                          ///< Shared chess table.
    Game start_game_;                                      ///< Game logic using the table.
    RunningGame running_game_;                              ///< RunningGame interface.

    std::map<int, std::shared_ptr<RunningGame>> games_;    ///< Map of active games.
    std::mutex game_mutex_;                                ///< Mutex for thread-safe access to games.
    std::condition_variable game_condition_;              ///< Condition variable to wait for game start.
};