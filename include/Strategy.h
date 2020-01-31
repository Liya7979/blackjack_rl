#ifndef STRATEGY_H
#define STRATEGY_H

#include <random>
#include <algorithm>

#include "DefaultDict.h"
#include "State.h"

namespace Strategy {

    double random_between_0_1();

    int random_action();

    int epsilon_greedy(double epsilon, DefaultDict<StateAction, double> &qtable, const State &state);

    int best_action(DefaultDict<StateAction, double> &qtable, const State &state);

    void update_qtable(int reward, std::vector<StateAction> &occurred_state_actions, DefaultDict<StateAction,
            double> &qtable, DefaultDict<State, int> &state_count, DefaultDict<StateAction, int> &state_action_count,
                       const std::string& method, double gamma = 0.8);
}


#endif //STRATEGY_H
