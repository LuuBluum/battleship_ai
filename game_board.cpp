#pragma once

#include "battleship_board.hpp"
#include "coordinate.cpp"

#include <iostream>
#include <cstring>

class game_board
{
private:
    const unsigned int x_dimension;
    const unsigned int y_dimension;
    battleship_board board;

public:
    void display_board() const
    {
        for(int i = 0; i < y_dimension; ++i)
        {
            for(int j = 0; j < x_dimension; ++j)
            {
                coordinate temp_coord(j,i);
                std::cout << board.get_value(temp_coord.get_string());
            }
            std::cout << std::endl;
        }
    }

    bool get_shot()
    {
        std::string input;
        std::cout << "Please specify a shot, or type 'display' to display the board." << std::endl;
        std::cin >> input;
        std::string result;
        if(input == "display")
            display_board();
        else
            result = board.shoot(input);
        
        if(result == "miss")
            std::cout << "You hit nothing but water." << std::endl;
        else if(result == "hit")
             std::cout << "You manage to hit a ship!" << std::endl;
        else if (!result.empty())
             std::cout << "You sunk their " + result + "!" << std::endl;
        else
            return get_shot();

        return board.won();
    }

    game_board(const unsigned int& x, const unsigned int& y, const battleship_board& b): x_dimension(x), y_dimension(y), board(b) {};
};