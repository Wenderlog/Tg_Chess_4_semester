//
// Created by Кирилл Грибанов  on 01/04/2025.
//

#include "Server_Interface.h"

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
    try {
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

        // сохраняем актуальное состояние доски конкретной игры
        std::string new_state = manager_.GetBoardState(game_id);
        database_.UpdateGameHistory(game_id, new_state);

        res.set_content(success ? "Move accepted" : "Invalid move", "text/plain");
    } catch (const std::exception &e) {
        res.set_content(std::string("Error: ") + e.what(), "text/plain");
    }
});

    svr.Get("/status", [&](const httplib::Request &req, httplib::Response &res) {
    try {
        int player_id = std::stoi(req.get_param_value("id_player"));

        std::lock_guard<std::mutex> lock(game_mutex);

        if (!player_map.count(player_id)) {
            res.set_content("You are not authenticated!", "text/plain");
            return;
        }

        int game_id = database_.GetGameIDByPlayerID(player_id);

        pqxx::work txn(*database_.conn_);
        pqxx::result r = txn.exec(
            "SELECT board_states "
            "FROM GameHistory "
            "WHERE game_id = " + txn.esc(std::to_string(game_id))
        );

        std::string board_array;
        if (!r.empty() && !r[0][0].is_null()) {
            board_array = r[0][0].as<std::string>(); // преобразует массив PostgreSQL в строку
        }

        std::cout << board_array << std::endl;
        res.set_content("Board history:\n" + board_array, "text/plain");
    } catch (const std::exception &e) {
        res.set_content(std::string("Error: ") + e.what(), "text/plain");
    }
});

    svr.listen("0.0.0.0", 9090);
}