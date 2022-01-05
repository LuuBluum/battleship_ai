#pragma once

#include <vector>
#include <string>
#include <set>

#include "ship.hpp"
#include "coordinate.cpp"

class battleship_board
{
private:
    std::vector<ship> ships_remaining;
    std::set<coordinate> hits;
    std::set<coordinate> shots;

public:
    bool won() const { return ships_remaining.empty(); };
    std::string shoot(const std::string);
    char get_value(const std::string) const;

    battleship_board(std::vector<ship> given_ships): ships_remaining(given_ships) {};
};