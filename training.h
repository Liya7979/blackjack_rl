#ifndef TRAINING_H
#define TRAINING_H

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <array>

#include "DefaultDict.h"
#include "Blackjack.h"
#include "State.h"
#include "Strategy.h"

namespace training {
    DefaultDict<StateAction, double> train(int training_iter, int testing_iter, const std::string &method,
                                           std::deque<char> deck, int init_cards_num, int winning_points,
                                           int dealer_crit_points_to_stick);
    template<typename T>
    bool vec_contains(std::vector<T> &vec, T& obj) {
        return (std::find(vec.begin(), vec.end(), obj) != vec.end());
    }
}

#endif //TRAINING_H
