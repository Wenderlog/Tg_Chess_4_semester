//
// Created by Кирилл Грибанов  on 10/07/2025.
//

#include "DataBase.h"

DataBase::DataBase(const std::string& conn_str) {
    try {
        conn_ = std::make_unique<pqxx::connection>(conn_str);
        if (conn_->is_open()) {
            std::cout << "The connection is successful." << std::endl;
        } else {
            std::cerr << "Connection error." << std::endl;
            conn_.reset();
        }
    } catch (const std::exception& e) {
        std::cerr << "An exception was raised when connecting: " << e.what() << std::endl;
        conn_.reset();
    }
}

pqxx::result DataBase::Execute(const std::string& query) {
    try {
        if (!conn_ || !conn_->is_open()) {
            throw std::runtime_error("Connection to the database is not established");
        }

        pqxx::work txn(*conn_);
        pqxx::result res = txn.exec(query);
        txn.commit();

        for (const auto& row : res) {
            if (row.size() > 0) {
                std::cout << "Row: ";
                for (std::size_t i = 0; i < res.columns(); ++i) {
                    std::cout << res.column_name(i) << "=" << row[i].c_str() << " ";
                }
                std::cout << std::endl;
            }
        }

        return res;
    }
    catch (const std::exception &e) {
        std::cerr << "Request execution error: " << e.what() << std::endl;
        throw;
    }
}

std::string DataBase::DetermineUserColor(int user_id) {
    return (user_id % 2 == 0) ? "black" : "white";
}

int DataBase::InsertIDToDataBase(int user_id, const std::string& username, int game_id) {
    try {
        if (!conn_ || !conn_->is_open()) {
            std::cerr << "Connection to the database is not established" << std::endl;
            return 1;
        }

        std::string userColour = (user_id % 2 == 0) ? "black" : "white";

        std::string query =
            "INSERT INTO \"User\" (user_id, username, game_id, userColour) VALUES (" +
            std::to_string(user_id) + ", " +
            "'" + conn_->esc(username) + "', " +
            std::to_string(game_id) + ", '" +
            userColour + "');";

        pqxx::work txn(*conn_);
        txn.exec(query);
        txn.commit();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "User insertion error:" << e.what() << std::endl;
        return 1;
    }
}

void DataBase::GetPlayerID(const std::string& username) {
    try {
        std::string query = "SELECT user_id FROM \"User\" WHERE username = '" + conn_->esc(username) + "';";
        pqxx::result res = Execute(query);

        if (!res.empty()) {
            int user_id = res[0]["user_id"].as<int>();
            std::cout << "User ID:" << username << " это " << user_id << std::endl;
        } else {
            std::cout << "The user was not found: " << username << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in finding the user: " << e.what() << std::endl;
    }
}

void DataBase::GetGameID(const std::string& username) {
    try {
        std::string query = "SELECT game_id FROM \"User\" WHERE username = '" + conn_->esc(username) + "';";
        pqxx::result res = Execute(query);

        if (!res.empty()) {
            int game_id = res[0]["game_id"].as<int>();
            std::cout << "Game ID for player " << username << " is " << game_id << std::endl;
        } else {
            std::cout << "User not found: " << username << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in GetGameID: " << e.what() << std::endl;
    }
}

int DataBase::DeleteIDFromDataBase(const std::string& username) {
    try {
        std::string query = "DELETE FROM \"User\" WHERE username = '" + conn_->esc(username) + "';";
        pqxx::work txn(*conn_);
        txn.exec(query);
        txn.commit();
        std::cout << "User " << username << " deleted successfully." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting user: " << e.what() << std::endl;
        return 1;
    }
}

void DataBase::CreateNewGame(int game_id, const std::string& initial_board) {
    try {
        if (!conn_ || !conn_->is_open()) {
            throw std::runtime_error("Connection to the database is not established");
        }

        pqxx::work txn(*conn_);
        std::string query = "INSERT INTO GameHistory (game_id, board_states) VALUES (" +
                            txn.esc(std::to_string(game_id)) + ", ARRAY[" +
                            txn.quote(initial_board) + "])";
        txn.exec(query);
        txn.commit();

        std::cout << "A new game has been created with game_id:" << game_id << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error when creating a new game: " << e.what() << std::endl;
        throw;
    }
}

void DataBase::UpdateGameHistory(int game_id, const std::string& new_board_state) {
    try {
        if (!conn_ || !conn_->is_open()) {
            throw std::runtime_error("Connection to the database is not established");
        }

        pqxx::work txn(*conn_);

        std::string query =
            "UPDATE GameHistory "
            "SET board_states = ARRAY[" + txn.quote(new_board_state) + "] "
            "WHERE game_id = " + txn.esc(std::to_string(game_id));

        txn.exec(query);
        txn.commit();

        std::cout << "Game history is overwritten for game_id: " << game_id << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error updating the game history: " << e.what() << std::endl;
        throw;
    }
}

int DataBase::DeleteGame(int game_id) {
    try {
        if (!conn_ || !conn_->is_open()) {
            throw std::runtime_error("Connection to the database is not established");
        }

        pqxx::work txn(*conn_);

        std::string deleteUsersQuery =
            "DELETE FROM \"User\" WHERE game_id = " + txn.esc(std::to_string(game_id));
        txn.exec(deleteUsersQuery);

        std::string deleteGameQuery =
            "DELETE FROM GameHistory WHERE game_id = " + txn.esc(std::to_string(game_id));
        txn.exec(deleteGameQuery);

        txn.commit();

        std::cout << "Playing with game_id " << game_id << " successfully deleted" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error deleting the game: " << e.what() << std::endl;
        return 1;
    }
}

int DataBase::GetGameIDByPlayerID(int user_id) {
    try {
        if (!conn_ || !conn_->is_open()) {
            throw std::runtime_error("Connection to the database is not established");
        }

        std::string query =
            "SELECT game_id FROM \"User\" WHERE user_id = " + std::to_string(user_id) + " LIMIT 1;";

        pqxx::result res = Execute(query);

        if (!res.empty()){
            return res[0]["game_id"].as<int>();
        } else {
            std::cerr << "⚠️ User with user_id= " << user_id << " not found in the database." << std::endl;
            return -1;
        }
    } catch (const std::exception &e) {
        std::cerr << "❌ Error in GetGameIDByPlayerID: " << e.what() << std::endl;
        return -1;
    }
}