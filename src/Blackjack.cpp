#include "../include/Blackjack.h"
#include <algorithm>
#include <random>

Blackjack::Blackjack(std::deque<char> &deck, int initial_cards, int winning_points) :
        deck(deck),
        initial_cards(initial_cards),
        winning_points(winning_points) {
    finished = false;

    std::shuffle(this->deck.begin(), this->deck.end(), std::mt19937(std::random_device()()));

    fill_hand(player_cards);
    fill_hand(dealer_cards);
    dealer_first_card_points = get_card_points(dealer_cards.front());

    auto player_hand = calculate_hand_points(player_cards);
    player_points = player_hand.first;
    player_usable_ace = player_hand.second;

    auto dealer_hand = calculate_hand_points(dealer_cards);
    dealer_points = dealer_hand.first;
}

void Blackjack::fill_hand(std::deque<char> &hand) {
    for (int i = 0; i < initial_cards; ++i) {
        char card = draw_card();
        hand.push_back(card);
    }
}

char Blackjack::draw_card() {
    char card = deck.front();
    deck.pop_front();
    return card;
}

void Blackjack::player_draw_card() {
    char card = draw_card();
    player_cards.push_back(card);
}

void Blackjack::dealer_draw_card() {
    char card = draw_card();
    dealer_cards.push_back(card);
}

int Blackjack::get_card_points(const char card) {
    if (std::isdigit(card)) {
        return card == '1' ? 10 : card - '0';
    } else if (card == 'A') {
        return 1;
    } else {
        return 10;
    }
}

std::pair<int, bool> Blackjack::calculate_hand_points(const std::deque<char> &hand) {
    int points = 0, ace_quantity = 0;
    bool usable_ace = false;
    for (char card: hand) {
        points += get_card_points(card);
        if (card == 'A') {
            points += 10;
            ++ace_quantity;
        }
    }
    while (ace_quantity > 0 && points > this->winning_points) {
        --ace_quantity;
        points -= 10;
    }
    if (ace_quantity > 0) {
        usable_ace = true;
    }
    return std::make_pair(points, usable_ace);
}


Round::Round(int player, int dealer, bool usable_ace, int reward) :
        player(player),
        dealer(dealer),
        usable_ace(usable_ace),
        reward(reward) {}

// HELPERS. NOT NEEDED ANYWHERE ELSE
int cmp(int a, int b) {
    return (a > b) - (a < b);
}

void calculate_reward(int &reward, int player_points, int dealer_points, int winning_points) {
    if (dealer_points > winning_points) {
        reward = 1;
    } else if (player_points <= winning_points) {
        reward = cmp(player_points, dealer_points);
    }
}
// END HELPERS

Round step(Blackjack &game, int action, int dealer_critical_points_to_stick) {
    int reward = -2, // Reward is undefined
            player_points = game.player_points,
            dealer_points = game.dealer_points;
    bool usable_ace = game.player_usable_ace;

    if (action == 0) {
        game.player_draw_card();
        auto player_hand_points = game.calculate_hand_points(game.player_cards);
        player_points = player_hand_points.first;
        usable_ace = player_hand_points.second;
        if (player_points > game.winning_points) {
            reward = -1;
        }
    } else {
        while (dealer_points < dealer_critical_points_to_stick) {
            game.dealer_draw_card();
            auto dealer_hand_points = game.calculate_hand_points(game.dealer_cards);
            dealer_points = dealer_hand_points.first;
        }
        calculate_reward(reward, player_points, dealer_points, game.winning_points);
    }
    Round round(player_points, dealer_points, usable_ace, reward);
    return round;
}
