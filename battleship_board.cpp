#include "battleship_board.hpp"

#include <iostream>

std::string battleship_board::shoot(const std::string target_location)
{
    shots.insert(coordinate::make_coordinate(target_location));
    for(auto i = ships_remaining.begin(); i != ships_remaining.end(); ++i)
    {
        if(i->get_squares().count(coordinate::make_coordinate(target_location)))
        {
            hits.insert(coordinate::make_coordinate(target_location));
            bool ship_sunk = true;
            for(auto sq : i->get_squares())
            {
                if(!shots.count(sq))
                    ship_sunk = false;
            }
            if(ship_sunk)
            {
                std::string name_of_ship = i->label;
                ships_remaining.erase(i);
                return name_of_ship;
            }
            return "hit";
        }
    }
    return "miss";
};

char battleship_board::get_value(const std::string target_location) const
{
    if(shots.count(coordinate::make_coordinate(target_location)))
    {
        if(hits.count(coordinate::make_coordinate(target_location)))
            return 'h';
        return 'm';
    }
    return '.';
};