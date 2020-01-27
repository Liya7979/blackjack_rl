//
// Created by liya on 1/18/20.
//

#ifndef BLACKJACK_BLACKJACK_H
#define BLACKJACK_BLACKJACK_H

#include <string>
#include <random>
#include <utility>
#include <vector>
#include <algorithm>
#include <cctype>
#include <climits>
#import <iostream>

struct State_action {
    int player_points;
    int dealer_first_card_points;
    int usable_ace;
    int action;
    int reward;

    State_action(int player_points, int dealer_first_card_points, int usable_ace, int action, int reward=0) :
            player_points(player_points), dealer_first_card_points(dealer_first_card_points),
            usable_ace(usable_ace),reward(reward), action(action) {

    }

    bool operator==(const State_action &other) const {
        return player_points == other.player_points &&
               dealer_first_card_points == other.dealer_first_card_points &&
               usable_ace == other.usable_ace &&
               action == other.action;
    }
};

struct State {
    int player_points;
    int dealer_first_card_points;
    int usable_ace;

    State(int player_points, int dealer_first_card_points, int usable_ace) :
            player_points(player_points), dealer_first_card_points(dealer_first_card_points),
            usable_ace(usable_ace) {

    }

    bool operator==(const State &other) const {
        return player_points == other.player_points &&
               dealer_first_card_points == other.dealer_first_card_points &&
               usable_ace == other.usable_ace;
    }
};

namespace std {
    template<>
    struct hash<State_action> {
        std::size_t operator()(const State_action &k) const {
            return k.player_points + 10 * k.dealer_first_card_points + 100 * k.usable_ace + 1000 * k.action;
        }
    };

    template<>
    struct hash<State> {
        std::size_t operator()(const State &k) const {
            return k.player_points + 10 * k.dealer_first_card_points + 100 * k.usable_ace;
        }

    };
}

class Blackjack {
private:
    std::vector<char> deck_content;
public:
    bool finish;
    int player_points;
    int dealer_points;
    int player_usable_ace;
    int dealer_first_card_points;

    std::vector<char> player_cards;
    std::vector<char> dealer_cards;

    Blackjack(std::vector<char> deck_content, int initial_cards = 2, int winning_points = 21);
    void shuffle_deck();
    char draw_card();
    static int calculate_dealer_first_card(std::vector<char> &cards);
    static std::pair<int, int> calculate_card_points(std::vector<char> &cards, int winning_points);
    int initial_cards;
    int winning_points;
};
struct Episode {
    int player;
    int dealer;
    int usable_ace;
    int reward;
    bool done;
    Episode(int player, int dealer, int usable_ace,  int reward, bool done) :
            player(player), dealer(dealer), usable_ace(usable_ace), reward(reward), done(done) {
    }
};

bool is_bust(int hand, int winning);
int calculate_reward(int player, int dealer, int winning_points);
int calculate_score(int hand, int winning);
int cmp (int a, int b);
Episode game_proceed(Blackjack &game, int action, int winning_points, int dealer_critical_points_to_stick);
State reset_game(Blackjack &game);
#endif //BLACKJACK_BLACKJACK_H
