//
// Created by liya on 1/26/20.
//

#ifndef BLACKJACK_STRATEGY_H
#define BLACKJACK_STRATEGY_H


#include <random>
#include <unordered_map>
#include <iostream>

struct Blackjack_state_action {
    int player_points;
    int dealer_first_card_points;
    int usable_ace;
    int action;

    Blackjack_state_action(int player_points, int dealer_first_card_points, int usable_ace, int action) :
            player_points(player_points), dealer_first_card_points(dealer_first_card_points),
            usable_ace(usable_ace),
            action(action) {

    }

    bool operator==(const Blackjack_state_action &other) const {
        return player_points == other.player_points &&
               dealer_first_card_points == other.dealer_first_card_points &&
               usable_ace == other.usable_ace &&
               action == other.action;
    }
};

struct Blackjack_state {
    int player_points;
    int dealer_first_card_points;
    int usable_ace;

    Blackjack_state(int player_points, int dealer_first_card_points, int usable_ace) :
            player_points(player_points), dealer_first_card_points(dealer_first_card_points),
            usable_ace(usable_ace) {

    }

    bool operator==(const Blackjack_state &other) const {
        return player_points == other.player_points &&
               dealer_first_card_points == other.dealer_first_card_points &&
               usable_ace == other.usable_ace;
    }
};

namespace std {
    template<>
    struct hash<Blackjack_state_action> {
        std::size_t operator()(const Blackjack_state_action &k) const {
            return k.player_points + 10 * k.dealer_first_card_points + 100 * k.usable_ace + 1000 * k.action;
        }
    };

    template<>
    struct hash<Blackjack_state> {
        std::size_t operator()(const Blackjack_state &k) const {
            return k.player_points + 10 * k.dealer_first_card_points + 100 * k.usable_ace;
        }

    };
}

namespace strategy {
    bool map_contains(std::unordered_map<Blackjack_state_action, double> &qtable, Blackjack_state_action &action);
    int random_action();
    int best_action(std::unordered_map<Blackjack_state_action, double> &qtable, int player_points,
                    int dealer_points, int player_usable_ace);
    int epsilon_greedy(double epsilon, std::unordered_map<Blackjack_state_action, double> &qtable, int player_points,
                       int dealer_points, int player_usable_ace);
    void update_qtable(int reward, std::vector<Blackjack_state_action> &occured_state_actions,
                       std::unordered_map<Blackjack_state_action, double> &qtable,
                       std::unordered_map<Blackjack_state, int> &state_count,
                       std::unordered_map<Blackjack_state_action, int> &state_action_count,
                       const std::string &method, double gamma = 0.8);

}


#endif //BLACKJACK_STRATEGY_H
