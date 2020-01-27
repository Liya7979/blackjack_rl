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

    void update_qtable(int reward, std::vector<State_action> &occured_state_actions,
                       std::unordered_map<State_action, double> &qtable,
                       std::unordered_map<State, int> &state_count,
                       std::unordered_map<State_action, int> &state_action_count,
                       const std::string &method, double gamma) {
        for (size_t i = 0; i < occured_state_actions.size(); i++) {
            State_action state_action = occured_state_actions[i];
            State state = State(state_action.player_points, state_action.dealer_first_card_points,
                                state_action.usable_ace);
            if (state_count.find(state) == state_count.end()) {
                state_count.insert(std::make_pair(state, 1));
            } else {
                state_count.at(state)++;
            }
            if (state_action_count.find(state_action) == state_action_count.end()) {
                state_action_count.insert(std::make_pair(state_action, 1));
            } else {
                state_action_count.at(state_action)++;
            }
            double alpha = 1.0 / state_action_count.at(state_action);
            int next_reward;
            if (method == "Q-learning" || method == "Sarsa") {
                double prev_q = qtable_get(qtable, state_action);
                double next_q;
                if (i < occured_state_actions.size() - 1) {
                    if (method == "Q-learning") {
                        State_action next_hit_state_action = occured_state_actions[i + 1];
                        next_hit_state_action.action = 0;
                        State_action next_stick_state_action = occured_state_actions[i + 1];
                        next_stick_state_action.action = 1;
                        double max_value = std::max(qtable_get(qtable, next_hit_state_action),
                                                    qtable_get(qtable, next_stick_state_action));
                        next_reward = max_value == qtable_get(qtable, next_hit_state_action) ?
                                      next_hit_state_action.reward : next_stick_state_action.reward;
                        next_q = gamma * max_value;
                    } else {
                        // Sarsa
                        State_action next_state_action = occured_state_actions[i + 1];
                        next_reward = next_state_action.reward;
                        next_q = gamma * qtable_get(qtable, next_state_action);
                    }
                } else {
                    next_q = 0;
                    next_reward = 0;
                }
                if (map_contains(qtable, occured_state_actions[i])) {
                    qtable.at(occured_state_actions[i]) += prev_q + alpha * (next_reward + next_q
                                                                            - prev_q);
                } else {
                    qtable.insert(std::make_pair(occured_state_actions[i],
                                                 prev_q + alpha * (next_reward + next_q
                                                                   - prev_q)));
                }
            } else {
                // TD
                auto key = occured_state_actions[i];
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

}
