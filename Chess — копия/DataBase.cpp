//
// Created by Кирилл Грибанов  on 10/07/2025.
//

#include "DataBase.h"

DataBase::DataBase(const std::string& conn_str) {
    try {
        conn_ = std::make_unique<pqxx::connection>(conn_str);
        if (conn_->is_open()) {
            std::cout << "Соединение успешно." << std::endl;
        } else {
            std::cerr << "Ошибка соединения." << std::endl;
            conn_.reset();
        }
    } catch (const std::exception& e) {
        std::cerr << "Вызвано исключение при подключении: " << e.what() << std::endl;
        conn_.reset();
    }
}

pqxx::result DataBase::Execute(const std::string& query) {
    try {
        if (!conn_ || !conn_->is_open()) {
            throw std::runtime_error("Соединение с БД не установлено");
        }

        pqxx::work txn(*conn_);
        pqxx::result res = txn.exec(query);
        txn.commit();

        // Вывод результата для отладки (можно убрать)
        for (const auto& row : res) {
            std::cout << "ID: " << row["id"].as<int>()
                      << ", Username: " << row["username"].c_str() << std::endl;
        }

        return res;
    }
    catch (const std::exception &e) {
        std::cerr << "Ошибка выполнения запроса: " << e.what() << std::endl;
        throw;  // или можно вернуть пустой результат, зависит от логики
    }
}

std::string DataBase::DetermineUserColor(int user_id) {
    return (user_id % 2 == 0) ? "black" : "white";
}

int DataBase::InsertIDToDataBase(int user_id, const std::string& username, int game_id) {
    try {
        if (!conn_ || !conn_->is_open()) {
            std::cerr << "Соединение с БД не установлено" << std::endl;
            return 1; // ошибка соединения
        }

        // Определяем цвет пользователя
        std::string userColour = (user_id % 2 == 0) ? "black" : "white";

        // Формируем SQL-запрос с параметрами
        std::string query =
            "INSERT INTO \"User\" (user_id, username, game_id, userColour) VALUES (" +
            std::to_string(user_id) + ", " +
            "'" + conn_->esc(username) + "', " +
            std::to_string(game_id) + ", '" +
            userColour + "');";

        pqxx::work txn(*conn_);
        txn.exec(query);
        txn.commit();

        return 0; // успех
    } catch (const std::exception& e) {
        std::cerr << "Ошибка вставки пользователя: " << e.what() << std::endl;
        return 1; // ошибка выполнения запроса
    }
}

// Получить ID игрока по username
void DataBase::GetPlayerID(const std::string& username) {
    try {
        std::string query = "SELECT user_id FROM \"User\" WHERE username = '" + conn_->esc(username) + "';";
        pqxx::result res = Execute(query);

        if (!res.empty()) {
            int user_id = res[0]["user_id"].as<int>();
            std::cout << "ID пользователя:" << username << " это " << user_id << std::endl;
        } else {
            std::cout << "Пользователь не найден: " << username << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка в нахождении пользователя: " << e.what() << std::endl;
    }
}

// Получить ID игры по имени игрока
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

// Удалить пользователя из базы по имени
int DataBase::DeleteIDFromDataBase(const std::string& username) {
    try {
        std::string query = "DELETE FROM \"User\" WHERE username = '" + conn_->esc(username) + "';";
        pqxx::work txn(*conn_);
        txn.exec(query);
        txn.commit();
        std::cout << "User " << username << " deleted successfully." << std::endl;
        return 0; // успех
    } catch (const std::exception& e) {
        std::cerr << "Error deleting user: " << e.what() << std::endl;
        return 1; // ошибка
    }
}

//переписать логику, добавив к User логин или почту, тогда все будет работать.

void DataBase::CreateNewGame(int game_id, const std::string& initial_board) {
    try {
        if (!conn_ || !conn_->is_open()) {
            throw std::runtime_error("Соединение с БД не установлено");
        }

        pqxx::work txn(*conn_);
        std::string query = "INSERT INTO GameHistory (game_id, board_states) VALUES (" +
                            txn.esc(std::to_string(game_id)) + ", ARRAY[" +
                            txn.quote(initial_board) + "])";
        txn.exec(query);
        txn.commit();

        std::cout << "Создана новая игра с game_id: " << game_id << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при создании новой игры: " << e.what() << std::endl;
        throw;
    }
}

void DataBase::UpdateGameHistory(int game_id, const std::string& new_board_state) {
    try {
        if (!conn_ || !conn_->is_open()) {
            throw std::runtime_error("Соединение с БД не установлено");
        }

        pqxx::work txn(*conn_);

        // Добавляем новое состояние доски в конец массива board_states
        std::string query =
            "UPDATE GameHistory "
            "SET board_states = array_append(board_states, " + txn.quote(new_board_state) + ") " // по идее пихнуть в new_board_state результат функции GenerateBoardState из Table
            "WHERE game_id = " + txn.esc(std::to_string(game_id));

        txn.exec(query);
        txn.commit();

        std::cout << "Обновлена история игры с game_id: " << game_id << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при обновлении истории игры: " << e.what() << std::endl;
        throw;
    }
}

int DataBase::DeleteGame(int game_id) {
    try {
        if (!conn_ || !conn_->is_open()) {
            throw std::runtime_error("Соединение с БД не установлено");
        }

        pqxx::work txn(*conn_);

        // Удаляем записи из User, связанные с этой игрой (если нужно)
        std::string deleteUsersQuery =
            "DELETE FROM \"User\" WHERE game_id = " + txn.esc(std::to_string(game_id));
        txn.exec(deleteUsersQuery);

        // Удаляем запись игры из GameHistory
        std::string deleteGameQuery =
            "DELETE FROM GameHistory WHERE game_id = " + txn.esc(std::to_string(game_id));
        txn.exec(deleteGameQuery);

        txn.commit();

        std::cout << "Игра с game_id " << game_id << " успешно удалена" << std::endl;
        return 0; // успех
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при удалении игры: " << e.what() << std::endl;
        return 1; // ошибка
    }
}

int DataBase::GetGameIDByPlayerID(int user_id) {
    try {
        if (!conn_ || !conn_->is_open()) {
            throw std::runtime_error("Соединение с БД не установлено");
        }

        std::string query =
            "SELECT game_id FROM \"User\" WHERE user_id = " + std::to_string(user_id) + " LIMIT 1;";

        pqxx::result res = Execute(query);

        if (!res.empty()){
            return res[0]["game_id"].as<int>();
        } else {
            std::cerr << "⚠️ Пользователь с user_id = " << user_id << " не найден в базе." << std::endl;
            return -1;  // Не найден
        }
    } catch (const std::exception &e) {
        std::cerr << "❌ Ошибка в GetGameIDByPlayerID: " << e.what() << std::endl;
        return -1;  // Ошибка
    }
}