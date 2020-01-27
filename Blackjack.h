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

class Blackjack {
private:
    std::vector<char> deck_content;
    int initial_cards;
    int winning_points;
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


};
struct episode {
    int player_points;
    int dealer_points;
    int usable_ace;
    int reward;
    bool done;

    episode(int player_points, int dealer_points, int usable_ace, int reward, bool done) :
            player_points(player_points), dealer_points(dealer_points), usable_ace(usable_ace), reward(reward), done(done) {
    }

};
bool is_bust(int hand, int winning);
int calculate_reward(int player, int dealer, int winning_points);
int calculate_score(int hand, int winning);
int cmp (int a, int b);
episode game_proceed(Blackjack &game, int action, int winning_points, int dealer_critical_points_to_stick);

#endif //BLACKJACK_BLACKJACK_H
