//
// Created by liya on 1/18/20.
//


#include "Blackjack.h"

#include <utility>



Blackjack::Blackjack(std::vector<char> deck_content, int initial_cards, int winning_points) :
        deck_content(std::move(deck_content)), initial_cards(initial_cards), winning_points(winning_points) {

    finish = false;
    player_points = 0;
    dealer_points = 0;
    player_usable_ace = 0;
    dealer_first_card_points = 0;
    shuffle_deck();
    for (int i = 0; i < initial_cards; i++) {
        player_cards.push_back(draw_card());
    }
    for (int i = 0; i < initial_cards; i++) {
        dealer_cards.push_back(draw_card());
        if (i == 0) {
            dealer_first_card_points = calculate_dealer_first_card(dealer_cards);
        }
    }
    std::pair<int, int> player = calculate_card_points(player_cards, winning_points);
    player_points = player.first;
    player_usable_ace = player.second;
    std::pair<int, int> dealer = calculate_card_points(dealer_cards, winning_points);
    dealer_points = dealer.first;
}

void Blackjack::shuffle_deck() {
    std::shuffle(deck_content.begin(), deck_content.end(), std::mt19937(std::random_device()()));
}

char Blackjack::draw_card() {
    char popped = deck_content[0];
    deck_content.erase(deck_content.begin());
    return popped;
}

int Blackjack::calculate_dealer_first_card(std::vector<char> &cards) {
    if (std::isdigit(cards[0])) {
        if (cards[0] == '1') {
            return 10;
        } else { return cards[0] - '0'; }
    } else if (cards[0] == 'A') {
        return 1;
    } else {
        return 10;
    }
}

std::pair<int, int> Blackjack::calculate_card_points(std::vector<char> &cards, int winning_points) {
    int points = 0;
    int ace_quantity = 0;
    int usable_ace = 0;
    for (auto &card: cards) {
        if (std::isdigit(card)) {
            if (card == '1') {
                points += 10;
            } else {
                points += (card - '0');
            }
        } else if (card == 'A') {
            points += 11;
            ace_quantity++;
        } else {
            points += 10;
        }
    }

    while (ace_quantity > 0 && points > winning_points) {
        ace_quantity--;
        points -= 10;
    }
    if (ace_quantity > 0) {
        usable_ace = 1;
    }
    return std::make_pair(points, usable_ace);
}
bool is_bust(int hand, int winning){
    return hand > winning;
}
int calculate_reward(int player, int dealer, int winning_points){
    return cmp(calculate_score(player, winning_points), calculate_score(dealer, winning_points));
}
int cmp (int a, int b){
    return int((a > b)) - int((a < b));
}
int calculate_score(int hand, int winning){
    int score = is_bust(hand, winning) ? 0 : hand;
    return score;
}
Episode game_proceed(Blackjack &game, int action, int winning_points, int dealer_critical_points_to_stick) {
    int reward;
    int player_points = game.player_points;
    int dealer_points = game.dealer_points;
    int player_usable_ace = game.player_usable_ace;
    bool done;
    if (action == 0) {
        game.player_cards.push_back(game.draw_card());
        std::pair<int, int> player_card_points = game.calculate_card_points(game.player_cards, winning_points);
        player_points = player_card_points.first;
        player_usable_ace = player_card_points.second;
        if (is_bust(player_points, winning_points)) {
            reward = -1;
            done = true;
        } else {
            done = false;
            reward = 0;
        }
    } else {
        done = true;
        while (dealer_points < dealer_critical_points_to_stick) {
            game.dealer_cards.push_back(game.draw_card());
            std::pair<int, int> dealer_card_points = game.calculate_card_points(game.dealer_cards, winning_points);
            dealer_points = dealer_card_points.first;
        }
        reward = calculate_reward(player_points, dealer_points, winning_points);
    }
    Episode e = Episode(player_points, dealer_points, player_usable_ace, reward, done);
    return e;

}




