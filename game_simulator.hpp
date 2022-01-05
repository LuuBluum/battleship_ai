#pragma once

#include <random>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>

#include "ship_locations.hpp"

class game_simulator
{
private:
    const unsigned int              x_dimension;
    const unsigned int              y_dimension;
    const std::vector<std::string>  ship_names;
    const std::vector<unsigned int> ship_lengths;
    const double                    error = 0.00001;

    ship_locations locations;
    std::default_random_engine* const generator;
    std::vector<std::vector<ship>> ship_arrangements;
    std::set<coordinate> hits;
    std::vector<std::vector<std::vector<ship>>> hit_permutations;
    std::unordered_set<ship> known_ships;

    std::set<coordinate>        generate_board()           const;
    std::map<coordinate,double> generate_probabilities()   const;

    void set_up_ships();

public:
    const bool        won()                                const;
    const std::string recommend_square_to_hit();
    void              get_reply(std::string&,std::string&);

    game_simulator(const unsigned int& x, const unsigned int& y, const std::vector<std::string>& names, const std::vector<unsigned int>& lengths, std::default_random_engine* gen): x_dimension(x)
                                                                                                                                                                                  , y_dimension(y)
                                                                                                                                                                                  , ship_names(names)
                                                                                                                                                                                  , ship_lengths(lengths)
                                                                                                                                                                                  , locations(x,y,names,lengths)
                                                                                                                                                                                  , generator(gen)
                                                                                                                                                                                  , ship_arrangements(locations.get_ships()) {};
};