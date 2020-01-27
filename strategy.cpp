//
// Created by liya on 1/25/20.
//

#include <random>
#include <unordered_map>
#include "strategy.h"

namespace strategy {
    bool map_contains(std::unordered_map<State_action, double> &qtable, State_action &action) {
        return (qtable.find(action) != qtable.end());
    }

    double qtable_get(std::unordered_map<State_action, double> &qtable, State_action &key) {
        if (map_contains(qtable, key)) {
            return qtable.at(key);
        } else {
            return 0.0;
        }
    }

    int random_action() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);
        if (dis(gen) <= 0.5) { return 0; } else { return 1; }
    }

    int best_action(std::unordered_map<State_action, double> &qtable, int player_points,
                    int dealer_points, int player_usable_ace) {
        State_action state(player_points, dealer_points, player_usable_ace, 0);
        double hit_val = qtable_get(qtable, state);
        state = State_action(player_points, dealer_points, player_usable_ace, 1);
        double stick_val = qtable_get(qtable, state);
        if (hit_val > stick_val) {
            return 0;
        } else if (stick_val > hit_val) {
            return 1;
        } else {
            return random_action();
        }
    }

    int epsilon_greedy(double epsilon, std::unordered_map<State_action, double> &qtable, int player_points,
                       int dealer_points, int player_usable_ace) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);
        if (dis(gen) < epsilon) {
            return random_action();
        } else {
            return best_action(qtable, player_points, dealer_points, player_usable_ace);
        }
    }

    void update_qtable(Episode next, int action, State state,
                       std::unordered_map<State_action, double> &qtable,
                       const std::string &method, double gamma) {
            State_action state_action = State_action(state.player_points,
                    state.dealer_first_card_points, state.usable_ace, action);
            double alpha = 0.02;
            int next_reward = next.reward;
            if (method == "Q-learning" || method == "Sarsa") {
                double current_q = qtable_get(qtable, state_action);
                double next_q;
                    if (method == "Q-learning") {
                        State_action next_hit_state_action = State_action(next.player,
                                                                          next.dealer,
                                                                          next.usable_ace, 0);
                        State_action next_stick_state_action = State_action(next.player,
                                                                            next.dealer,
                                                                            next.usable_ace, 1);
                        double max_value = std::max(qtable_get(qtable, next_hit_state_action),
                                                    qtable_get(qtable, next_stick_state_action));
                        next_q = gamma * max_value;
                    } else {
                        // Sarsa
                        int next_action = epsilon_greedy(0.01, qtable, next.player,
                                                         next.dealer, next.usable_ace);
                        State_action next_state_action = State_action(next.player,
                                                                      next.dealer,
                                                                      next.usable_ace, next_action);
                        next_q = gamma * qtable_get(qtable, next_state_action);
                    }

                double value = (1 - alpha) * current_q + alpha * (next_reward + next_q);
                if (map_contains(qtable, state_action)) {
                    qtable.at(state_action) = value;
                } else {
                    qtable.insert(std::make_pair(state_action, value));
                }
            } else {
                // TD
                auto key = state_action;
                next_reward = key.reward;
                auto value = qtable_get(qtable, key);
                if (map_contains(qtable, key)) {
                    qtable.at(key) += alpha * (next_reward - value);
                } else {
                    qtable.insert(std::make_pair(key, value + (alpha * (next_reward - value))));
                }
            }
    }

}
