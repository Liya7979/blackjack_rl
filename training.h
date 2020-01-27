//
// Created by liya on 1/26/20.
//

#ifndef BLACKJACK_TRAINING_H
#define BLACKJACK_TRAINING_H

#include <unordered_map>
#include <iostream>
#include "strategy.h"
#include "Blackjack.h"
#include <algorithm>
#include <climits>

namespace training {
    std::unordered_map<State_action, double> train(int training_iteration, int testing_iteration,
                                                   const std::string& method, std::vector<char> &deck_content,
                                                   int initial_number_cards, int winning_points,
                                                   int dealer_criticial_points_to_stick);
}
#endif //BLACKJACK_TRAINING_H
