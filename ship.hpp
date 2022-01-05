#pragma once

#include "coordinate.cpp"

#include <string>
#include <set>
#include <unordered_set>
#include <cassert>

struct ship
{
    coordinate   pos;         //coordinate of starting location
    unsigned int length;      //must be greater than 1
    bool         orientation; //0 for to the right of (pos_x,pos_y), 1 for down from (pos_x,pos_y)
    std::string  label;       //name of ship

    const std::set<coordinate> get_squares() const; //returns unordered_set of coordinates for each square occupied by this ship

    bool operator==(const ship&)             const; //returns equality comparison of ships based on equivalence of base position, orientation, and label                              

    size_t operator()(void)            const {}; //necessary for string hashing of ships
    bool operator()(const ship& other) const {}; //similarly necessary for string hashing of ships

    ship(const unsigned int& x, const unsigned int& y, const unsigned int& l, const bool& ori, const std::string& name): pos(x,y), length(l), orientation(ori), label(name)
    {
        assert((length > 0) && "Error: Length was less than or equal to 0.\n");
    }
};

namespace std
{
    template<>
    struct hash<ship>
    {
        inline size_t operator()(const ship& s) const //converts ship position, orientation, and label into string hash
        {
            std::hash<string> string_hash;
            std::string ori;
            s.orientation ? ori = "down" : ori = "right";
            return string_hash(s.pos.get_string() + ori + s.label);
        };
    };
}