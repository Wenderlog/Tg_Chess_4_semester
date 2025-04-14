#include <iostream>
#include <httplib.h>
#include <thread>

#include "/Users/wenderlender/Desktop/Chess/Server_Interface.h"

int main() {
    ChessServer server;
    std::thread server_thread(&ChessServer::runServer, &server);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    try {
        server_thread.join();
    } catch (const std::exception &e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}