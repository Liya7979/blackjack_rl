#include <vector>
#include <unordered_map>
#include <fstream>
#include "training.h"

using namespace std;

void send_to_file(const std::string &method, std::unordered_map<State_action, double> &qtable) {
    ofstream qt;
    std::string name = method + ".txt";
    qt.open(name);
    for (auto &x: qtable) {
        State_action action = x.first;
        qt << action.player_points << "," <<
           action.dealer_first_card_points << "," <<
           action.usable_ace << "," <<
           action.action << "," <<
           x.second << std::endl;
    }
    qt.close();
}

int main() {
    std::vector<char> default_deck = {'A', 'A', 'A', 'A', '2', '2', '2', '2', '3', '3', '3', '3',
                                      '4', '4', '4', '4', '5', '5', '5', '5', '6', '6', '6', '6',
                                      '7', '7', '7', '7', '8', '8', '8', '8', '9', '9', '9', '9',
                                      '1', '1', '1', '1', 'J', 'J', 'J', 'J', 'Q', 'Q', 'Q', 'Q', 'K', 'K', 'K', 'K'};
    int initial_number_cards = 2;
    int winning_points = 21;
    int dealer_criticial_points_stick = 17;
    std::unordered_map<State_action, double> qtable_QL = training::train(1000000, 100000,
                                                                         "Q-learning",
                                                                         default_deck,
                                                                         initial_number_cards,
                                                                         winning_points,
                                                                         dealer_criticial_points_stick);
//    std::unordered_map<State_action, double> qtable_Sarsa = training::train(1000000, 100000,
//                                                                            "Sarsa",
//                                                                            default_deck,
//                                                                            initial_number_cards,
//                                                                            winning_points,
//                                                                            dealer_criticial_points_stick);
//    std::unordered_map<State_action, double> qtable_TD = training::train(1000000, 100000,
//                                                                         "TD",
//                                                                         default_deck,
//                                                                         initial_number_cards,
//                                                                         winning_points,
//                                                                         dealer_criticial_points_stick);
    send_to_file("ql", qtable_QL);
    return 0;
}
