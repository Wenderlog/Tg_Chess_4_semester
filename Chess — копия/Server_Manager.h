//
// Created by Кирилл Грибанов  on 05/04/2025.
//

#pragma once

#include <mutex>
#include <atomic>
#include <thread>
#include <unordered_map>
#include "My_Blocking_Queue.h"
#include "Types/DataBase_types.h"
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include "My_Blocking_Queue.h"
#include "DataBase.h"
#include "Game.h"
#include "Run.h"

class idGenerator {
    public:
   explicit idGenerator (int id);
   int NextID();


  private:
    std::atomic<int> id_;
  };

/*!
 * \class Games_Manager
 * \brief Управляет активными шахматными играми и взаимодействием с базой данных.
 */
class Games_Manager {
public:
    /*!
     * \brief Конструктор с параметром подключения к базе данных.
     * \param db_conn_str Строка подключения к PostgreSQL.
     */
    explicit Games_Manager(const std::string& db_conn_str)
        : id_generator_(1),                 // Запускаем генератор ID с 1
          database_(db_conn_str),          // Подключаем БД
          game_started_(false),            // Изначально игра не начата
          table_(),                        // Инициализируем доску
          start_game_(table_),             // Game зависит от table_
          running_game_()                  // Если используется по умолчанию
    {}

    /*!
     * \brief Генерация новой игры и получение её ID.
     * \return Новый уникальный ID игры.
     */
    int GenerateGames();

    /*!
     * \brief Получить игру по ID.
     * \param id_game ID игры.
     * \return Умный указатель на игру, если найдена.
     */
    std::shared_ptr<RunningGame> GetGame(int id_game);

private:
    idGenerator id_generator_;  ///< Генератор уникальных ID.
    DataBase database_;         ///< Объект работы с БД.

    std::atomic<bool> game_started_;  ///< Флаг запуска игры.
    Table table_;                     ///< Общая доска (если требуется).
    Game start_game_;                 ///< Логика игры, использует table_.
    RunningGame running_game_;        ///< Интерфейс хода (если используется напрямую).

    std::map<int, std::shared_ptr<RunningGame>> games_;  ///< Карта активных игр.
    std::mutex game_mutex_;                              ///< Мьютекс доступа к играм.
    std::condition_variable game_condition_;             ///< Для ожидания старта игры.
};
