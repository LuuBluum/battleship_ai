#include "ship.hpp"

const std::set<coordinate> ship::get_squares() const //fetches squares occupied by a ship
{
    std::set<coordinate> output_set;
    if(orientation)                                         //Ship is oriented down
    {
        for(unsigned int i = pos.y; i < pos.y + length; ++i)
        {
            coordinate temp(pos.x,i);
            output_set.insert(temp);
        }
    }
    else                                                    //Ship is oriented to the right
    {
        for(unsigned int i = pos.x; i < pos.x + length; ++i)
        {
            coordinate temp(i,pos.y);
            output_set.insert(temp);
        }
    }
    return output_set;
};

bool ship::operator==(const ship& rhs) const
{
    return (pos.x == rhs.pos.x) && (pos.y == rhs.pos.y) && (orientation == rhs.orientation) && (label == rhs.label);
};