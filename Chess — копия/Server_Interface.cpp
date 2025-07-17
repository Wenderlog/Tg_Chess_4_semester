//
// Created by Кирилл Грибанов  on 01/04/2025.
//

#include "Server_Interface.h"
#include <iostream>
/*
void ChessServer::doAuth(const httplib::Request &req, httplib::Response &res) {
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
}
void ChessServer::runServer() { // передать базу данных по ссылке для общения
    httplib::Server svr;

    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("Chess Server is running!", "text/plain");
    });

    std::cout << "✅ Server started at http://localhost:9090\n";

    svr.Post("/auth", [&](const httplib::Request &req, httplib::Response &res) {
        this->doAuth(req, res);
    });

    // Move
    svr.Post("/move", [&](const httplib::Request &req, httplib::Response &res) {
        std::string player_id = req.get_param_value("id_player");
        std::string move = req.get_param_value("move");

        std::lock_guard<std::mutex> lock(game_mutex);

        if (player_map.find(player_id) == player_map.end()) {
            res.set_content("You are not authenticated!", "text/plain");
            return;
        }

        std::string color = player_map[player_id];
        Colour current_turn = table.GetCurrentTurn();

        if ((color == "White" && current_turn != Colour::WHITE) ||
            (color == "Black" && current_turn != Colour::BLACK)) {
            res.set_content("It's not your turn!", "text/plain");
            return;
        }

        auto coords = manager_.WordToCoord(table.getBoard(), move);

        if (coords.first.row == 8 || coords.second.row == 8) {
            res.set_content("Invalid coordinates!", "text/plain");
            return;
        }

        auto turnVerdict = table.CheckTurn(coords.first, coords.second);
        if (turnVerdict != Table::TurnVerdict::correct) {
            res.set_content("Invalid move!", "text/plain");
            return;
        }

        table.DoTurn(coords.first, coords.second);
        current_turn = (current_turn == Colour::WHITE) ? Colour::BLACK : Colour::WHITE;
        res.set_content("Move accepted: " + move + ". Now it's " + (current_turn == Colour::WHITE ? "White" : "Black") + "'s turn.", "text/plain");
    });

    // Game status
    svr.Get("/status", [&](const httplib::Request &, httplib::Response &res) {
        std::lock_guard<std::mutex> lock(game_mutex);

        Colour current_turn = table.GetCurrentTurn();
        std::string turn_str = (current_turn == Colour::WHITE) ? "White" : "Black";
        res.set_content("Current turn: " + turn_str +
                        "\nWhite: " + (white_player_id.empty() ? "None" : white_player_id) +
                        "\nBlack: " + (black_player_id.empty() ? "None" : black_player_id), "text/plain");
    });


    svr.Get("/board", [&](const httplib::Request &, httplib::Response &res) {
        std::lock_guard<std::mutex> lock(game_mutex);
        std::string boardState = table.GenerateBoardState();
        res.set_content(boardState, "text/plain");
    });

    if (!svr.listen("0.0.0.0", 9090)) {
        std::cerr << "❌ Server failed to start on port 9090\n";
    }
}
*/

void ChessServer::runServer() {
    httplib::Server svr;

    std::cout << "✅ Server started at http://localhost:9090\n";

    svr.Post("/auth", [&](const httplib::Request &req, httplib::Response &res) {
        std::string username = req.get_param_value("username");
        std::string email = req.get_param_value("email");

        std::lock_guard<std::mutex> lock(game_mutex);

        int player_id = id_generator.NextID();
        std::string color = (player_id % 2 == 0) ? "Black" : "White";

        // Найдём или создадим ожидающую игру
        if (!pending_game_id.has_value()) {
            // создаём игру и сохраняем id
            pending_game_id = manager_.GenerateGames();
        }

        int game_id = *pending_game_id;
        database_.InsertIDToDataBase(player_id, username, game_id);
        player_map[player_id] = color;

        if (!game_players.count(game_id)) {
            game_players[game_id] = {};
        }
        game_players[game_id].push_back(player_id);

        // Когда 2 игрока в игре — запускаем её
        if (game_players[game_id].size() == 2) {
            pending_game_id = std::nullopt;  // освобождаем слот под новую игру
        }

        res.set_content("Authenticated: ID = " + std::to_string(player_id) + ", Color = " + color, "text/plain");
    });

    svr.Post("/move", [&](const httplib::Request &req, httplib::Response &res) {
        int player_id = std::stoi(req.get_param_value("id_player"));
        std::string move = req.get_param_value("move");

        std::lock_guard<std::mutex> lock(game_mutex);

        if (!player_map.count(player_id)) {
            res.set_content("You are not authenticated!", "text/plain");
            return;
        }

        int game_id = database_.GetGameIDByPlayerID(player_id);
        auto game = manager_.GetGame(game_id);

        if (!game) {
            res.set_content("Game not found", "text/plain");
            return;
        }

        bool success = game->HandleMove(move, database_.DetermineUserColor(player_id));
        res.set_content(success ? "Move accepted" : "Invalid move", "text/plain");
    });

    svr.Get("/status", [&](const httplib::Request &req, httplib::Response &res) {
        int player_id = std::stoi(req.get_param_value("id_player"));

        std::lock_guard<std::mutex> lock(game_mutex);

        if (!player_map.count(player_id)) {
            res.set_content("You are not authenticated!", "text/plain");
            return;
        }

        int game_id = database_.GetGameIDByPlayerID(player_id);
        auto game = manager_.GetGame(game_id);
        if (!game) {
            res.set_content("Game not found", "text/plain");
            return;
        }

        std::string board = table.GenerateBoardState();
        res.set_content("Board state:\n" + board, "text/plain");
    });

    svr.listen("0.0.0.0", 9090);
}