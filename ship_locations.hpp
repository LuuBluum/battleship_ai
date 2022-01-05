#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

#include "ship.hpp"
#include "coordinate.cpp"


class ship_locations //Class which holds all possible locations of all ships
{
private:
    const unsigned int              x_dimension;
    const unsigned int              y_dimension;
    const std::vector<std::string>  ship_names;   //Indexed in same order as ship_lengths
    const std::vector<unsigned int> ship_lengths; //Indexed in same order as ship_names

    class specific_ship_location //Class which holds all possible locations of a single ship
    {
    private:
        const unsigned int                       length;
        const std::string                        label;
        
        std::unordered_multimap<coordinate,ship> ship_positions; //Maps specific locations to the ships that occupy those locations
    public:
        std::unordered_set<ship> get_ships() const;

        void remove_position(const coordinate&); // Removes all ships from the set of possible ship locations that include this position
        void remove_ship(const ship&);           // Removes this specific ship configuration from the set of all possible ship locations

        specific_ship_location(const unsigned int& l, const std::string& name, const ship_locations& locations):length(l), label(name)
        {
            for(unsigned int i = 0; i < locations.x_dimension; ++i)
            {
                for(unsigned int j = 0; j < locations.y_dimension; ++j)
                {
                    if(i+length < locations.x_dimension)
                    {
                        ship temp_ship(i,j,length,0,label);
                        auto points = temp_ship.get_squares();
                        for(auto p : points)
                            ship_positions.emplace(p,temp_ship);
                    }
                    if(j+length < locations.y_dimension)
                    {
                        ship temp_ship(i,j,length,1,label);
                        auto points = temp_ship.get_squares();
                        for(auto p : points)
                            ship_positions.emplace(p,temp_ship);
                    }
                }
            }
        };
    };

    std::unordered_map<std::string,specific_ship_location> ship_possibilities;

public:
    std::vector<std::vector<ship>> get_ships() const;  //Returns vector of vector of all ship possibilities

    void remove_position(const coordinate&);  //Remove all ships that pass through this square
    void remove_ship(const ship&);            //Remove specific ship possibility

    ship_locations(const unsigned int& x, const unsigned int& y, const std::vector<std::string>& names, const std::vector<unsigned int>& lengths): x_dimension(x), y_dimension(y), ship_names(names), ship_lengths(lengths)
    {
        assert((x_dimension > 0)                          && "Error: x dimension equals 0.\n");
        assert((y_dimension > 0)                          && "Error: y dimension equals 0.\n");
        assert((ship_names.size() == ship_lengths.size()) && "Error: provided a different number of ship lengths and ship names.\n");

        for(unsigned int i = 0; i < ship_names.size(); ++i)
            ship_possibilities.emplace(ship_names.at(i),specific_ship_location(ship_lengths.at(i),ship_names.at(i),*this));
    };
};