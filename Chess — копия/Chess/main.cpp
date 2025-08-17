#include <httplib.h>
#include <iostream>
#include <pqxx/pqxx>
#include <thread>

#include "/Users/wenderlender/Desktop/Chess/Server_Interface.h"

int main() {
    ChessServer server;

    // Running the server in a separate thread
    std::thread server_thread(&ChessServer::runServer, &server);

    // Optional: we give the server time to start
    std::this_thread::sleep_for(std::chrono::seconds(1));

    try {
        // Waiting for the server to shut down (usually the server will not shut down on its own)
        server_thread.join();
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
