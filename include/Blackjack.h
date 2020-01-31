#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <deque>

struct Blackjack {
    std::deque<char> deck, player_cards, dealer_cards;
    bool finished;
    int player_points, dealer_points;
    bool player_usable_ace;
    int dealer_first_card_points;
    int initial_cards;
    int winning_points;


    Blackjack(std::deque<char> &deck, int initial_cards = 2, int winning_points = 21);

    void fill_hand(std::deque<char> &hand);

    char draw_card();
    void player_draw_card();
    void dealer_draw_card();

    static int get_card_points(char card);

    std::pair<int, bool> calculate_hand_points(const std::deque<char> &hand);

};

struct Round {
    int player, dealer, reward;
    bool usable_ace;

    Round(int player, int dealer, bool usable_ace, int reward);

};

Round step(Blackjack &game, int action, int dealer_critical_points_to_stick);

#endif //BLACKJACK_H
