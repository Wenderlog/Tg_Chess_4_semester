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


class ChessServer {
public:
    ChessServer() = default;
    void runServer();

private:
    std::mutex game_mutex;  // Потокобезопасность
    Table table;            // Таблица игры
    Manager manager_;       // Менеджер для обработки ходов
    std::map<std::string, std::string> player_map;  // Карта игроков
    std::string white_player_id;
    std::string black_player_id;
};

