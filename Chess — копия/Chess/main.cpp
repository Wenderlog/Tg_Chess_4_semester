//  main.cpp
//  Chess
//
//  Created by Кирилл Грибанов on 05.10.2024.
//

#include <iostream>
#include <httplib.h>
#include <map>
#include <string>
#include <boost/algorithm/string.hpp>

#include "Game.h"
#include "Manager.h"
#include "Run.h"
#include "Table.h"

/*!
 * \brief Entry point for the Chess application.
 * \details The `main` function serves as the entry point for the Chess game application.
 * It initializes the `RunningGame` object, which manages the game loop, user input,
 * and game logic. The `RunningGame::Run()` method is called to start the game and
 * handle the main loop, including player moves, game state updates, and termination
 * conditions. In case of an error, an exception is caught, and an error message is printed.
 *
 * \return 0 if the game runs successfully, or 1 if an error occurs.
 */

#include <iostream>
#include <thread>
#include <memory>
#include <httplib.h>

void runServer() {

    httplib::Server svr;

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello from cpp-httplib!", "text/plain");
    });

    std::cout << "✅ Server started at http://localhost:9090\n";

    svr.Get("/turn", [&](const httplib::Request& req, httplib::Response& res) {
        std::string id_player = req.get_param_value("id_player");
        std::string id_game = req.get_param_value("id_game");
        std::string turn_player = req.get_param_value("turn_player");
        res.set_content("id_player: " + id_player + ", id_game: " + id_game + ", turn_player: " + turn_player, "text/plain");
    });

    if (!svr.listen("0.0.0.0", 9090)) {
        std::cerr << "❌ Server failed to start! Is the port 9090 already in use?" << std::endl;
    }
}

int main() {
    // Запускаем сервер в отдельном потоке
    std::thread server_thread(runServer);

    // Ждём, чтобы сервер успел запуститься
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Дальнейший код продолжает выполняться
    std::string text = "Hello, Boost!";
    boost::to_upper(text);
    std::cout << text << std::endl;

    try {
        RunningGame runningGame;
        int result = runningGame.Run();

        server_thread.detach();
        return result;

    } catch (const std::exception& e) {
        std::cerr << "❌ An error occurred: " << e.what() << std::endl;
        server_thread.detach();
        return 1;
    }

    return 0;
}