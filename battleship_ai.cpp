#include "battleship_ai.hpp"

const std::string battleship_ai::run_turn()
{
    std::string output;
    auto target = simulator.recommend_square_to_hit();
    output += "The AI targets " + target;
    auto result = board->shoot(target);
    if(result == "miss")
        output += " and hits nothing but water.\n";
    else if(result == "hit")
        output += " and manages to hit a ship!\n";
    else
        output += " and sinks your " + result + "!\n";

    simulator.get_reply(target,result);
    return output;
};