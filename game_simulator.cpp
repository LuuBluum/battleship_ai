#include "game_simulator.hpp"

#include <cmath>
#include <queue>
#include <iterator>
#include <sstream>
#include <algorithm>

#include <iostream>

std::set<coordinate> game_simulator::generate_board() const
{
    unsigned int total_length = 0;
    for(auto s : ship_lengths)
        total_length += s;
    std::set<coordinate> ship_squares;
    do
    {
        ship_squares.clear();
        std::unordered_set<std::string> used_ships;
        if(!hit_permutations.empty()) //Should there already be a hit, then the logic becomes more nuanaced
        {
            std::uniform_int_distribution<int> distribution(0,hit_permutations.size()-1);
            auto chosen_hit_permutation = hit_permutations.at(distribution(*generator));
            for(auto individual_hit : chosen_hit_permutation)
            {
                std::uniform_int_distribution<int> specific_distribution(0,individual_hit.size()-1);
                ship chosen_ship = individual_hit.at(specific_distribution(*generator));
                used_ships.insert(chosen_ship.label);
                for(auto square : chosen_ship.get_squares())
                    ship_squares.insert(square);
            }
        }
        for(auto individual_ship_arrangement : ship_arrangements)
        {
            if(used_ships.count(individual_ship_arrangement.at(0).label))
                continue;
            std::uniform_int_distribution<int> distribution(0,individual_ship_arrangement.size()-1);
            ship chosen_ship = individual_ship_arrangement.at(distribution(*generator));
            for(auto square : chosen_ship.get_squares())
                ship_squares.insert(square);
        }
    }
    while(ship_squares.size() < total_length);
    return ship_squares;
};

std::map<coordinate,double> game_simulator::generate_probabilities() const
{
    class probability_board
    {
    private:
        const unsigned int        x_dimension;
        const unsigned int        y_dimension;
        std::vector<unsigned int> count_board;
        unsigned int              total_values;
    public:
        const double get_probability(coordinate coord) const { return total_values ? count_board.at(coord.x+coord.y*x_dimension)   /double(total_values) : 0;  };
        void         add_count(coordinate coord)             {               count_board.at(coord.x+coord.y*x_dimension)+=1; total_values++; };

        std::map<coordinate,double> get_board() const
        {
            std::map<coordinate,double> output_board;
            for(unsigned int i = 0; i < x_dimension; ++i)
                for(unsigned int j = 0; j < y_dimension; ++j)
                    output_board.emplace(coordinate(i,j),get_probability(coordinate(i,j)));

            return output_board;
        };

        probability_board(unsigned int x, unsigned int y):x_dimension(x), y_dimension(y), count_board(x*y), total_values(0)
        {
            for(unsigned int i = 0; i < x*y; ++i)
                count_board.push_back(0);
        };
    };

    probability_board board(x_dimension, y_dimension);
    double average_probability_change;
    do
    {
        average_probability_change = 0;
        probability_board old_board(board);
        for(auto coord : generate_board())
            board.add_count(coord);

        for(unsigned int i = 0; i < x_dimension; ++i)
            for(unsigned int j = 0; j < y_dimension; ++j)
                average_probability_change += abs(board.get_probability(coordinate(i,j))-old_board.get_probability(coordinate(i,j)));
        average_probability_change /= x_dimension*y_dimension;
    }
    while(average_probability_change > error);

    return board.get_board();
};

const std::string game_simulator::recommend_square_to_hit()
{
    const coordinate* choice = nullptr;
    double probability = 0;
    auto data_set = generate_probabilities();
    for(auto& square : data_set)
    {
        if(probability < square.second && !hits.count(square.first))
        {
            choice = &square.first;
            probability = square.second;
        }
    }

    return choice->get_string();
};

void game_simulator::get_reply(std::string& coord, std::string& result)
{
    if(result == "miss")
    {
        locations.remove_position(coordinate::make_coordinate(coord));
    }
    else if(result == "hit")
    {
        hits.emplace(coordinate::make_coordinate(coord));
    }
    else //If not a miss or a hit, then a ship name indicating that we sunk a ship; find out which and remove the rest
    {
        hits.emplace(coordinate::make_coordinate(coord));
        for(auto ship_vector : ship_arrangements)
        {
            if(ship_vector.at(0).label != result)
                continue;
            for(auto specific_ship : ship_vector)
            {
                bool all_hit = true;
                for(auto ship_square : specific_ship.get_squares())
                {
                    if(!hits.count(ship_square))
                    {
                        all_hit = false;
                        break;
                    }
                }
                if(all_hit)
                    known_ships.emplace(specific_ship);
                else
                    locations.remove_ship(specific_ship);                
            }
        }
    }
    set_up_ships();
}

void game_simulator::set_up_ships()
{
    ship_arrangements.clear();
    hit_permutations.clear();
    ship_arrangements = locations.get_ships();
    //We need to establish our hit configurations. First, iterate through every ship and put each one that hits into a pool

    std::multimap<std::string,ship> hit_ships; //Elements consist of a string of coordinates followed by the ship label as keys, with the respective ships as values
    std::set<std::string> hit_labels;          //Just the labels
    auto true_hits = hits;                     //We need a temporary set of hits because
    for(auto s : known_ships)                  //we want to remove all known ships from our selection
    {
        std::string ship_hit_label;
        for(auto sq : s.get_squares())
        {
            true_hits.erase(sq);
            ship_hit_label += sq.get_string() + " ";
        }
        hit_ships.emplace(ship_hit_label+s.label,s);
        hit_labels.emplace(ship_hit_label+s.label);
    }
    for(auto ship_type : ship_arrangements)    //With that done, we can now calculate the rest
    {
        for(auto s : ship_type)
        {
            std::string ship_hit_label;
            for(auto sq : s.get_squares())
            {
                if(true_hits.count(sq))
                    ship_hit_label += sq.get_string() + " ";
            }
            if(!ship_hit_label.empty())
            {
                hit_ships.emplace(ship_hit_label+s.label,s);
                hit_labels.emplace(ship_hit_label+s.label);
            }
        }
    }
    std::vector<std::string> hit_label_vector; //We need numerically limited labels,
    for(auto l : hit_labels)                   //so use our set and fill a vector
        hit_label_vector.push_back(l);         //which will prevent repeats

    std::queue<std::pair<std::vector<std::string>,int>> hit_subset_queue;
    std::vector<std::string> empty_set;
    hit_subset_queue.emplace(empty_set,0);

    std::unordered_set<std::string> ship_name_set; //Better lookup than a vector, and we'll be referencing it a lot
    for(auto n : ship_names)
        ship_name_set.insert(n);
    while(!hit_subset_queue.empty())
    {
        auto hit_arrangement = hit_subset_queue.front();
        hit_subset_queue.pop();

        std::set<coordinate>  hit_squares;
        std::set<std::string> used_ships;

        for(auto s : hit_arrangement.first)
        {
            std::stringstream ss(s);
            std::istream_iterator<std::string> begin(ss);
            std::istream_iterator<std::string> end;
            std::vector<std::string> deconstructed_hit_arrangement(begin,end);
            for(auto st : deconstructed_hit_arrangement)
                if(!ship_name_set.count(st))
                    hit_squares.insert(coordinate::make_coordinate(st));
                else
                    used_ships.insert(st);
        }
        if(hit_squares == hits) //If we've hit everything with this combination
        {
            std::vector<std::vector<ship>> hit_vector;
            for(auto specific_hit : hit_arrangement.first)
            {
                std::vector<ship> specific_hit_vector;
                auto ships = hit_ships.equal_range(specific_hit);
                std::for_each(ships.first, ships.second,[&specific_hit_vector](auto s)
                {
                    specific_hit_vector.push_back(s.second);
                });
                hit_vector.push_back(specific_hit_vector);
            }
            hit_permutations.push_back(hit_vector);
        }
        else if(used_ships.size() < ship_names.size())
        {
            for(unsigned int i = hit_arrangement.second; i < hit_label_vector.size(); ++i)
            {
                std::vector<coordinate> specific_hit_squares;
                std::string specific_used_ship;

                //Populate specific_hit_squares and specific_used_ships (latter should be singular)

                std::stringstream ss(hit_label_vector.at(i));
                std::istream_iterator<std::string> begin(ss);
                std::istream_iterator<std::string> end;
                std::vector<std::string> deconstructed_hit_arrangement(begin,end);
                for(auto st : deconstructed_hit_arrangement)
                    if(!ship_name_set.count(st))
                        specific_hit_squares.push_back(coordinate::make_coordinate(st));
                    else
                        specific_used_ship = st;
                bool is_valid = true;
                if(used_ships.count(specific_used_ship))
                    is_valid = false;
                for(auto sq : specific_hit_squares)
                {
                    if(hit_squares.count(sq))
                    {
                        is_valid = false;
                        break;
                    }
                }
                if(is_valid)
                {
                    std::vector<std::string> new_hits = hit_arrangement.first;
                    new_hits.push_back(hit_label_vector.at(i));
                    hit_subset_queue.emplace(new_hits,i+1);
                }
            }
        }     
    }
}

const bool game_simulator::won() const
{
    unsigned int total_length = 0;
    for(auto l : ship_lengths)
        total_length += l;
    if(hits.size() == total_length)
        return true;
    return false;
}