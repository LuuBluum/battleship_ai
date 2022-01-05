#pragma once

#include <string>
#include <cassert>
#include <vector>

struct coordinate
{
    unsigned int x,y;
    const std::string get_string() const
    {
        std::string to_return;
        int temp_x = x;
        do
        {
            to_return.push_back(char((temp_x % 26) + 65));
        }
        while ((temp_x-=26) > 0);   

        to_return += std::to_string(y+1);
        return to_return;        
    };

    bool operator==(const coordinate& rhs) const
    {
        return x==rhs.x && y==rhs.y;
    };

    bool operator<(const coordinate& rhs) const
    {
        if(x == rhs.x)
            return y<rhs.y;
        return x<rhs.x;
    };

    size_t operator()(void) const {};
    bool operator()(const coordinate& c) const {};

    coordinate(unsigned int pos_x, unsigned int pos_y): x(pos_x), y(pos_y) {};
    coordinate()                                      : coordinate(0,0)    {};

    static const coordinate make_coordinate(const std::string& coord)
    {
        unsigned int x = 0;
        unsigned int y = 0;
        //A coordinate's format should be a series of letters followed by a series of numbers, so we can iterate through and recreate the x and y values
        std::string y_string;

        std::vector<unsigned int> x_values;

        for(auto c : coord)
        {
            if(isalpha(c))
                x_values.push_back(c - 'A' + 1);   
            else
            {
                y_string = coord.substr(coord.find_first_of(c));
                break;
            }
        }
        
        for(int i = 0; i < x_values.size(); ++i)
            x += (x_values.at(i)-1)*(x_values.size()-i);

        y = stoi(y_string);    //Convert stringed int to int
        y--;                   //Y is offset by 1

        return coordinate(x,y);
    };
};

namespace std
{
    template<>
    struct hash<coordinate>
    {
        inline size_t operator()(const coordinate& c) const
        {
            std::hash<string> string_hash;
            return string_hash(c.get_string());
        };
    };
}