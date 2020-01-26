//
// Created by liya on 1/25/20.
//

#include <random>
#include <unordered_map>
#include "strategy.h"

namespace strategy {

    bool map_contains(std::unordered_map<Blackjack_state_action, double> &qtable, Blackjack_state_action &action) {
        return (qtable.find(action) != qtable.end());
    }

    double qtable_get(std::unordered_map<Blackjack_state_action, double> &qtable, Blackjack_state_action &key) {
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


    int best_action(std::unordered_map<Blackjack_state_action, double> &qtable, int player_points,
                    int dealer_points, int player_usable_ace) {
        Blackjack_state_action state(player_points, dealer_points, player_usable_ace, 0);
        double hit_val = qtable_get(qtable, state);
//        double hit_val = qtable.at(Blackjack_state_action(player_points, dealer_points, player_usable_ace, 0));
        state = Blackjack_state_action(player_points, dealer_points, player_usable_ace, 1);
        double stick_val = qtable_get(qtable, state);
//        double stick_val = qtable.at(Blackjack_state_action(player_points, dealer_points, player_usable_ace, 1));
        if (hit_val > stick_val) {
            return 0;
        } else if (stick_val > hit_val) {
            return 1;
        } else {
            return random_action();
        }
    }

    int epsilon_greedy(double epsilon, std::unordered_map<Blackjack_state_action, double> &qtable, int player_points,
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

    void update_qtable(int reward, std::vector<Blackjack_state_action> &occured_state_actions,
                       std::unordered_map<Blackjack_state_action, double> &qtable,
                       std::unordered_map<Blackjack_state, int> &state_count,
                       std::unordered_map<Blackjack_state_action, int> &state_action_count,
                       const std::string &method, double gamma) {
        for (size_t i = 0; i < occured_state_actions.size(); i++) {


            Blackjack_state_action state_action = occured_state_actions[i];
            Blackjack_state state = Blackjack_state(state_action.player_points, state_action.dealer_first_card_points,
                                                    state_action.usable_ace);

            if (state_count.find(state) == state_count.end()) {
                state_count.insert(std::make_pair(state, 1));
            } else {
                state_count.at(state)++;
//                state_count.insert(std::make_pair(state, state_count.at(state) + 1));
            }
            if (state_action_count.find(state_action) == state_action_count.end()) {
                state_action_count.insert(std::make_pair(state_action, 1));
            } else {
                state_action_count.at(state_action)++;
//                state_action_count.insert(std::make_pair(state_action,
//                                                         state_action_count.at(state_action) + 1));
            }
            double alpha = 1.0 / state_action_count.at(state_action);
            if (method == "Q-learning" || method == "Sarsa") {
                double previous_q_val = qtable_get(qtable, state_action);
                double best_next_q_value;

                if (i < occured_state_actions.size() - 1) {

                    if (method == "Q-learning") {

                        Blackjack_state_action next_hit_state_action = occured_state_actions[i + 1];
                        next_hit_state_action.action = 0;
                        Blackjack_state_action next_stick_state_action = occured_state_actions[i + 1];
                        next_stick_state_action.action = 1;
                        double max_value = std::max(qtable_get(qtable, next_hit_state_action),
                                                    qtable_get(qtable, next_stick_state_action));
                        best_next_q_value = gamma * max_value;
                    } else {
                        Blackjack_state_action next_state_action = occured_state_actions[i + 1];
                        best_next_q_value = gamma * qtable_get(qtable, next_state_action);
                    }
                } else {
                    best_next_q_value = 0;
                }
                if (map_contains(qtable, occured_state_actions[i])) {
                    qtable.at(occured_state_actions[i]) =
                            (1 - alpha) * previous_q_val + alpha * (reward + best_next_q_value);
                } else {
                    qtable.insert(std::make_pair(occured_state_actions[i],
                                                 (1 - alpha) * previous_q_val + alpha * (reward + best_next_q_value)));
                }
//                qtable[occured_state_actions[i]] = (1 - alpha) * previous_q_val + alpha * (reward + best_next_q_value);
            } else {
                auto key = occured_state_actions[i];
                auto value = qtable_get(qtable, key);
                if (map_contains(qtable, key)) {
                    qtable.at(key) += alpha * (reward - value);
                } else {
                    qtable.insert(std::make_pair(key, value + (alpha * (reward - value))));
                }
//                qtable[occured_state_actions[i]] += alpha * (reward - qtable[occured_state_actions[i]]);
//                qtable.insert(std::make_pair(key, value + (alpha * (reward - value))));
            }
        }
    }

}
