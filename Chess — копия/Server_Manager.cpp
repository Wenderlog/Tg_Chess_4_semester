//
// Created by Кирилл Грибанов  on 05/04/2025.
//

#include "Server_Manager.h"

idGenerator::idGenerator(int id) : id_(id) {}

int idGenerator::NextID() {
  return id_.fetch_add(1);
}

int Games_Manager::GenerateGames() {
    std::lock_guard<std::mutex> lock(game_mutex_);

    auto game = std::make_shared<RunningGame>();
    int id_game = id_generator_.NextID();

    games_[id_game] = game;

    std::string initial_board = table_.GenerateBoardState();
    database_.CreateNewGame(id_game, initial_board);

    boost::thread([this, game]() {
        start_game_.StartGame();
        game->Run();
    }).detach();

    return id_game;
}


std::shared_ptr<RunningGame> Games_Manager::GetGame(int id_game) {
    std::lock_guard<std::mutex> lock(game_mutex_);
    auto it = games_.find(id_game);
    if (it != games_.end())
        return it->second;
    return nullptr;
}

std::string Games_Manager::GetBoardState(int id_game) {
    std::lock_guard<std::mutex> lock(game_mutex_);

    auto it = games_.find(id_game);
    if (it != games_.end() && it->second) {
        return it->second->GetBoardState();
    }

    throw std::runtime_error("Game not found with id: " + std::to_string(id_game));
}



