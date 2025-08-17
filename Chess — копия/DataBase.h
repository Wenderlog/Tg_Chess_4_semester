//
// Created by Кирилл Грибанов  on 10/07/2025.
//

#pragma once

#include <iostream>
#include <pqxx/pqxx>
#include <string>

/**
 * @class DataBase
 * @brief Class for working with PostgreSQL database.
 *
 * Provides connection handling, SQL query execution, and management of players and games.
 */
class DataBase {
public:
  /**
   * @brief Pointer to the database connection.
   *
   * A shared_ptr is used to safely share the connection
   * between multiple objects or threads.
   */
  std::shared_ptr<pqxx::connection> conn_; ///< Pointer to the PostgreSQL connection.

  /**
   * @brief Inserts a user into the database and returns their ID.
   *
   * A game is created first, and then players join it.
   *
   * @param user_id Unique identifier of the user.
   * @param username Username of the player.
   * @param game_id Unique identifier of the game.
   * @return The ID assigned to the user.
   */
  int InsertIDToDataBase(int user_id, const std::string& username, int game_id);

  /**
   * @brief Retrieves the player ID by username.
   *
   * @param username Username of the player.
   */
  void GetPlayerID(const std::string& username);

  /**
   * @brief Retrieves the game ID by player's username.
   *
   * @param username Username of the player.
   */
  void GetGameID(const std::string& username);

  /**
   * @brief Deletes a user from the database by username.
   *
   * @param username Username of the player.
   * @return Operation result code (0 — success, 1 — error).
   */
  int DeleteIDFromDataBase(const std::string& username);

  /**
   * @brief Determines the player's color (white or black) by their ID.
   *
   * @param user_id Unique identifier of the player.
   * @return String with the player's color: "white" or "black".
   */
  std::string DetermineUserColor(int user_id);

  /**
   * @brief Creates a new game in the database.
   *
   * @param game_id Unique identifier of the game.
   * @param initial_board Initial state of the chessboard.
   */
  void CreateNewGame(int game_id, const std::string& initial_board);

  /**
   * @brief Updates the game history.
   *
   * @param game_id Unique identifier of the game.
   * @param new_board_state New board state after a move.
   */
  void UpdateGameHistory(int game_id, const std::string& new_board_state);

  /**
   * @brief Deletes a game from the database.
   *
   * @param game_id Unique identifier of the game.
   * @return Operation result code (0 — success, 1 — error).
   */
  int DeleteGame(int game_id);

  /**
   * @brief Constructor. Establishes a connection to the PostgreSQL database.
   *
   * @param conn_str PostgreSQL connection string.
   */
  explicit DataBase(const std::string& conn_str);

  /**
   * @brief Destructor. Automatically closes the connection.
   */
  ~DataBase() = default;

  /**
   * @brief Executes an SQL query and returns the result.
   *
   * @param query SQL query as a string.
   * @return Query execution result (pqxx::result).
   */
  pqxx::result Execute(const std::string& query);

  /**
   * @brief Retrieves the game ID by player ID.
   *
   * @param user_id Unique identifier of the player.
   * @return Unique identifier of the game.
   */
  int GetGameIDByPlayerID(int user_id);
};