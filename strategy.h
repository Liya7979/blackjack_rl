//
// Created by liya on 1/26/20.
//

#ifndef BLACKJACK_STRATEGY_H
#define BLACKJACK_STRATEGY_H


#include <random>
#include <unordered_map>
#include <iostream>
#include "Blackjack.h"



namespace strategy {
    bool map_contains(std::unordered_map<State_action, double> &qtable, State_action &action);

    int random_action();

    int best_action(std::unordered_map<State_action, double> &qtable, int player_points,
                    int dealer_points, int player_usable_ace);

    int epsilon_greedy(double epsilon, std::unordered_map<State_action, double> &qtable, int player_points,
                       int dealer_points, int player_usable_ace);

    void update_qtable(Episode next, int action, State state,
                       std::unordered_map<State_action, double> &qtable,
                       const std::string &method, double gamma = 1);

}


#endif //BLACKJACK_STRATEGY_H
