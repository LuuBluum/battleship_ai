#pragma once

#include <random>

#include "game_simulator.hpp"
#include "battleship_board.hpp"

class battleship_ai
{
private:
    game_simulator simulator;
    battleship_board* const board;

public:
    const std::string run_turn();
    const bool        won()      const { return simulator.won(); };

    battleship_ai(const unsigned int& x, const unsigned int& y, const std::vector<std::string>& names, const std::vector<unsigned int>& lengths, std::default_random_engine* gen, battleship_board* const b): simulator(x,y,names,lengths,gen), board(b) {};
};