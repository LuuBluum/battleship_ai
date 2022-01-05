#include "ship_locations.hpp"

#include <algorithm>

//ship_locations functions
std::vector<std::vector<ship>> ship_locations::get_ships() const
{
    std::vector<std::vector<ship>> output_vector;

    for(auto ship_type : ship_possibilities)
    {
        std::vector<ship> ship_vector;
        for(auto ship_position : ship_type.second.get_ships())
            ship_vector.push_back(ship_position);
        output_vector.push_back(ship_vector);
    }
    return output_vector;
};

void ship_locations::remove_position(const coordinate& position)
{
    for(auto& ship_type : ship_possibilities)
        ship_type.second.remove_position(position);
};

void ship_locations::remove_ship(const ship& ship_to_remove)
{
    ship_possibilities.at(ship_to_remove.label).remove_ship(ship_to_remove);
}

//specific_ship_locations functions
std::unordered_set<ship> ship_locations::specific_ship_location::get_ships() const
{
    std::unordered_set<ship> output_set;
    for(auto s : ship_positions)
        output_set.insert(s.second);
    return output_set;
};

void ship_locations::specific_ship_location::remove_position(const coordinate& position)
{
    auto ships_to_remove_range = ship_positions.equal_range(position);
    std::vector<ship> ships_to_remove;
    std::for_each(ships_to_remove_range.first, ships_to_remove_range.second,[&ships_to_remove](auto s)
    {
        ships_to_remove.push_back(s.second);
    });
    
    for(auto s : ships_to_remove)
        remove_ship(s);
    ship_positions.erase(position);
};

void ship_locations::specific_ship_location::remove_ship(const ship& ship_to_remove)
{
    auto i = ship_positions.begin();
    while(i != ship_positions.end())
        if(i->second == ship_to_remove)
            i = ship_positions.erase(i);
        else
            ++i;
}