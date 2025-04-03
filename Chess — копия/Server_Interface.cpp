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

    // Authentication
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

    // Move
    svr.Post("/move", [&](const httplib::Request &req, httplib::Response &res) {
        std::string player_id = req.get_param_value("id_player");
        std::string move = req.get_param_value("move"); // Example: e2e4

        std::lock_guard<std::mutex> lock(game_mutex);

        if (player_map.find(player_id) == player_map.end()) {
            res.set_content("You are not authenticated!", "text/plain");
            return;
        }

        std::string color = player_map[player_id];

        // Get the current turn from the Table object
        Colour current_turn = table.GetCurrentTurn();

        // Check if it is the player's turn
        if ((color == "White" && current_turn != Colour::WHITE) ||
            (color == "Black" && current_turn != Colour::BLACK)) {
            res.set_content("It's not your turn!", "text/plain");
            return;
        }

        // Convert the move string to coordinates
        auto coords = manager_.WordToCoord(table.getBoard(), move);

        // If coordinates are invalid
        if (coords.first.row == 8 || coords.second.row == 8) {
            res.set_content("Invalid coordinates!", "text/plain");
            return;
        }

        // Check if the move is valid
        auto turnVerdict = table.CheckTurn(coords.first, coords.second);
        if (turnVerdict != Table::TurnVerdict::correct) {
            res.set_content("Invalid move!", "text/plain");
            return;
        }

        // Execute the move
        table.DoTurn(coords.first, coords.second);

        // Change the current turn
        current_turn = (current_turn == Colour::WHITE) ? Colour::BLACK : Colour::WHITE;
        res.set_content("Move accepted: " + move + ". Now it's " + (current_turn == Colour::WHITE ? "White" : "Black") + "'s turn.", "text/plain");
    });

    // Game status
    svr.Get("/status", [&](const httplib::Request &, httplib::Response &res) {
        std::lock_guard<std::mutex> lock(game_mutex);

        // Get the current turn from the Table object
        Colour current_turn = table.GetCurrentTurn();
        std::string turn_str = (current_turn == Colour::WHITE) ? "White" : "Black";

        res.set_content("Current turn: " + turn_str +
                        "\nWhite: " + (white_player_id.empty() ? "None" : white_player_id) +
                        "\nBlack: " + (black_player_id.empty() ? "None" : black_player_id), "text/plain");
    });

    // Get board state
    svr.Get("/board", [&](const httplib::Request &, httplib::Response &res) {
        std::lock_guard<std::mutex> lock(game_mutex);

        // Get the string representation of the board
        std::string boardState = table.GenerateBoardState();

        // Send the board state as text
        res.set_content(boardState, "text/plain");
    });

    if (!svr.listen("0.0.0.0", 9090)) {
        std::cerr << "❌ Server failed to start on port 9090\n";
    }
}