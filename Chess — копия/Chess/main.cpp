#include <iostream>
#include <httplib.h>
#include <map>
#include <thread>
#include <string>
#include <mutex>

#include "/Users/wenderlender/Desktop/Chess/Game.h"
#include "/Users/wenderlender/Desktop/Chess/Manager.h"
#include "/Users/wenderlender/Desktop/Chess/Run.h"
#include "/Users/wenderlender/Desktop/Chess/Table.h"
#include "/Users/wenderlender/Desktop/Chess/Server_Interface.h"

int main() {
    // Создаем объект ChessServer
    ChessServer server;

    // Запускаем сервер в отдельном потоке
    std::thread server_thread(&ChessServer::runServer, &server);

    // Подождем, чтобы сервер успел запуститься
    std::this_thread::sleep_for(std::chrono::seconds(1));

    try {
        // Ожидаем завершения работы сервера
        server_thread.join();
    } catch (const std::exception &e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}