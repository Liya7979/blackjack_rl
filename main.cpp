#include <vector>
#include <unordered_map>
#include "training.h"

using namespace std;

int main() {
    std::vector<char> default_deck = {'A', 'A', 'A', 'A', '2', '2', '2', '2', '3', '3', '3', '3',
                                      '4', '4', '4', '4', '5', '5', '5', '5', '6', '6', '6', '6',
                                      '7', '7', '7', '7', '8', '8', '8', '8', '9', '9', '9', '9',
                                      '1', '1', '1', '1', 'J', 'J', 'J', 'J', 'Q', 'Q', 'Q', 'Q', 'K', 'K', 'K', 'K'};
    int initial_number_cards = 2;
    int winning_points = 21;
    int dealer_criticial_points_stick = 17;
    std::unordered_map<Blackjack_state_action, double> qtable_QL = training::train_test(1000000, 100000,
                                                                                        "Q-learning",
                                                                                        default_deck,
                                                                                        initial_number_cards,
                                                                                        winning_points,
                                                                                        dealer_criticial_points_stick);
//    std::unordered_map<Blackjack_state_action, double> qtable_Sarsa = training::train_test(1000, 1000,
//                                                                                            "Sarsa",
//                                                                                           default_deck,
//                                                                                           initial_number_cards,
//                                                                                           winning_points,
//                                                                                           dealer_criticial_points_stick);
//    std::unordered_map<Blackjack_state_action, double> qtable_TD = training::train_test(1000, 1000,
//                                                                                        "TD",
//                                                                                        default_deck,
//                                                                                        initial_number_cards,
//                                                                                        winning_points,
//                                                                                        dealer_criticial_points_stick);


    return 0;
}