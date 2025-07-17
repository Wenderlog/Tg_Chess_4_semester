#include <iostream>
#include <httplib.h>
#include <thread>
#include <pqxx/pqxx>

#include "/Users/wenderlender/Desktop/Chess/Server_Interface.h"

int main() {
    ChessServer server;

    // Запускаем сервер в отдельном потоке
    std::thread server_thread(&ChessServer::runServer, &server);

    // Опционально: даём серверу время на запуск
    std::this_thread::sleep_for(std::chrono::seconds(1));

    try {
        // Ждём завершения работы сервера (обычно сервер не завершится сам)
        server_thread.join();
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
//проблема что не создается игра и возможно они не параллелются ( ведь даже не создаются) либо не подключается