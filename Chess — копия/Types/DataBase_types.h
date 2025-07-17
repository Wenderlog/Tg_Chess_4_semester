//
// Created by Кирилл Грибанов  on 21/05/2025.
//

#pragma once

struct Turn {
    std::string from;
    std::string to;

};

struct TurnInfo {
    int player_id;
    int game_id;
    Turn turn;
};
