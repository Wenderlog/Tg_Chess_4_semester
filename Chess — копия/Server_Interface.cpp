//
// Created by Кирилл Грибанов  on 01/04/2025.
//

#include "Server_Interface.h"
#include <iostream>

void ChessServer::runServer() {
    httplib::Server svr;

    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("Chess Server is running!", "text/plain");
    });

    std::cout << "✅ Server started at http://localhost:9090\n";

    // Авторизация
    svr.Post("/auth", [&](const httplib::Request &req, httplib::Response &res) {
        std::string player_id = req.get_param_value("id_player");
        std::string color = req.get_param_value("color");

        std::lock_guard<std::mutex> lock(game_mutex);

        if (player_map.count(player_id)) {
            res.set_content("Player already authenticated!", "text/plain");
            return;
        }

        if (color == "White" && white_player_id.empty()) {
            white_player_id = player_id;
            player_map[player_id] = "White";
        } else if (color == "Black" && black_player_id.empty()) {
            black_player_id = player_id;
            player_map[player_id] = "Black";
        } else {
            res.set_content("Color already taken or invalid.", "text/plain");
            return;
        }

        res.set_content("Player authenticated: " + player_id + " as " + color, "text/plain");
    });

    // Ход
    svr.Post("/move", [&](const httplib::Request &req, httplib::Response &res) {
        std::string player_id = req.get_param_value("id_player");
        std::string move = req.get_param_value("move"); // Пример: e2e4

        std::lock_guard<std::mutex> lock(game_mutex);

        if (player_map.find(player_id) == player_map.end()) {
            res.set_content("You are not authenticated!", "text/plain");
            return;
        }

        std::string color = player_map[player_id];

        // Получаем текущий ход через объект Table
        Colour current_turn = table.GetCurrentTurn();

        // Проверка, что текущий ход соответствует игроку
        if ((color == "White" && current_turn != Colour::WHITE) ||
            (color == "Black" && current_turn != Colour::BLACK)) {
            res.set_content("It's not your turn!", "text/plain");
            return;
        }

        // Преобразуем строку хода в координаты
        auto coords = manager_.WordToCoord(table.getBoard(), move);

        // Если координаты неверны
        if (coords.first.row == 8 || coords.second.row == 8) {
            res.set_content("Invalid coordinates!", "text/plain");
            return;
        }

        // Проверка на допустимость хода
        auto turnVerdict = table.CheckTurn(coords.first, coords.second);
        if (turnVerdict != Table::TurnVerdict::correct) {
            res.set_content("Invalid move!", "text/plain");
            return;
        }

        // Выполняем ход
        table.DoTurn(coords.first, coords.second);

        // Меняем текущий ход
        current_turn = (current_turn == Colour::WHITE) ? Colour::BLACK : Colour::WHITE;
        res.set_content("Move accepted: " + move + ". Now it's " + (current_turn == Colour::WHITE ? "White" : "Black") + "'s turn.", "text/plain");
    });

    // Статус игры
    svr.Get("/status", [&](const httplib::Request &, httplib::Response &res) {
        std::lock_guard<std::mutex> lock(game_mutex);

        // Получаем текущий ход через объект Table
        Colour current_turn = table.GetCurrentTurn();
        std::string turn_str = (current_turn == Colour::WHITE) ? "White" : "Black";

        res.set_content("Current turn: " + turn_str +
                        "\nWhite: " + (white_player_id.empty() ? "None" : white_player_id) +
                        "\nBlack: " + (black_player_id.empty() ? "None" : black_player_id), "text/plain");
    });

    // Получение состояния доски
    svr.Get("/board", [&](const httplib::Request &, httplib::Response &res) {
        std::lock_guard<std::mutex> lock(game_mutex);

        // Получаем строковое представление доски
        std::string boardState = table.GenerateBoardState();

        // Отправляем строку доски как текст
        res.set_content(boardState, "text/plain");
    });

    if (!svr.listen("0.0.0.0", 9090)) {
        std::cerr << "❌ Server failed to start on port 9090\n";
    }
}