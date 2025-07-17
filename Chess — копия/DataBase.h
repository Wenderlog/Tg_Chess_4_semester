//
// Created by Кирилл Грибанов  on 10/07/2025.
//

#include <iostream>
#include <string>
#include <pqxx/pqxx>
#pragma once

/**
 * @class DataBase
 * @brief Класс для работы с базой данных PostgreSQL.
 * Обеспечивает подключение и выполнение SQL-запросов.
 */
class DataBase {
public:
  /**
   * @brief Указатель на подключение к базе данных.
   * Используется уникальный указатель для безопасной работы в многопоточной среде.
   */
  std::unique_ptr<pqxx::connection> conn_; // переделать на shared и попробовать как-то реализовать через ссылку

  /**
   * @brief Вставляет пользователя в базу данных и возвращает ID.
   * @param name Имя пользователя.
   * @return ID, присвоенный пользователю.
   */
  int InsertIDToDataBase(int user_id, const std::string& username, int game_id); // получилось так, что сначала создается игра, а только потом к ней присоединяются пользователи

  /**
   * @brief Получает ID игрока по имени.
   * @param username Имя пользователя.
   */
  void GetPlayerID(const std::string& username);

  /**
   * @brief Получает ID игры по имени игрока.
   * @param username Имя пользователя.
   */
  void GetGameID(const std::string& username);

  /**
   * @brief Удаляет пользователя из базы по имени.
   * @param username Имя пользователя.
   * @return Код результата операции (например, 0 — успех, 1 — ошибка).
   */
  int DeleteIDFromDataBase(const std::string& username);

  std::string DetermineUserColor(int user_id); // определение цвета игрока

  void CreateNewGame(int game_id, const std::string& initial_board);

  void UpdateGameHistory(int game_id, const std::string& new_board_state);

  int DeleteGame(int game_id);

  /**
   * @brief Конструктор, устанавливает соединение с базой данных.
   * @param conn_str Строка подключения PostgreSQL.
   */
  explicit DataBase(const std::string& conn_str);

  /**
   * @brief Деструктор. Автоматически закрывает соединение.
   */
  ~DataBase() = default;

  /**
   * @brief Выполняет SQL-запрос и возвращает результат.
   * @param query SQL-запрос в виде строки.
   * @return Результат выполнения запроса (pqxx::result).
   */
  pqxx::result Execute(const std::string& query);

  int GetGameIDByPlayerID(int user_id);

};
