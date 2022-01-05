#include "battleship_ai.hpp"
#include "battleship_board.hpp"
#include "ship.hpp"
#include "game_board.cpp"

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <random>
#include <chrono>
#include <fstream>
#include <cstring>

int main()
{
    std::vector<std::string> ship_names {"Carrier", "Battleship", "Cruiser","Submarine","Destroyer"};
    std::vector<unsigned int> ship_lengths {5,4,3,3,2};
    const int map_length_x = 10;
    const int map_length_y = 10;
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::vector<ship> ships;
    ship_locations location(map_length_x,map_length_y,ship_names,ship_lengths);
    auto ship_arrangements = location.get_ships();
    unsigned int total_length = 0;
    for(auto s : ship_lengths)
        total_length += s;
    std::set<coordinate> ship_squares;
    do
    {
        ship_squares.clear();
        ships.clear();
        for(auto individual_ship_arrangement : ship_arrangements)
        {
            std::uniform_int_distribution<int> distribution(0,individual_ship_arrangement.size()-1);
            ship chosen_ship = individual_ship_arrangement.at(distribution(generator));
            ships.push_back(chosen_ship);
            for(auto square : chosen_ship.get_squares())
                ship_squares.insert(square);
        }
    }
    while(ship_squares.size() < total_length);

    battleship_board board(ships);

    game_board b(map_length_x,map_length_y,board);
    ship_squares.clear();
    ships.clear();
    std::string placement_ship;
    std::cout << "Hello, and welcome to Battleship! To start, we'll need to set up a board. Your board will be provided to you at random." << std::endl;
    ship_squares.clear();
    ships.clear();
    do
    {
        ship_squares.clear();
        ships.clear();
        for(auto individual_ship_arrangement : ship_arrangements)
        {
            std::uniform_int_distribution<int> distribution(0,individual_ship_arrangement.size()-1);
            ship chosen_ship = individual_ship_arrangement.at(distribution(generator));
            ships.push_back(chosen_ship);
            for(auto square : chosen_ship.get_squares())
                ship_squares.insert(square);
        }
    }
    while(ship_squares.size() < total_length);

    battleship_board bo(ships);
    battleship_ai ai(10,10,ship_names,ship_lengths,&generator,&bo);

    bool anyone_won = false;
    bool player_won = false;
    while(!anyone_won)
    {
        player_won = b.get_shot();
        if(player_won)
        {
            anyone_won = true;
            continue;
        }
        std::cout << ai.run_turn();
        anyone_won = ai.won();
    }
    if(player_won)
        std::cout << "You won!" << std::endl;
    else
        std::cout << "You lost." << std::endl;
    return 0;
}