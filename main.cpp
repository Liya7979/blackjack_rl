#include <fstream>
#include "training.h"

void send_to_file(const std::string &method, DefaultDict<StateAction, double> &qtable) {
    std::ofstream qt;
    std::string name = method + ".txt";
    qt.open(name);
    for (auto &x: qtable.to_map()) {
        StateAction action = x.first;
        State state = action.get_state();
        qt << state.player_points << "," <<
           state.dealer_points << "," <<
           state.usable_ace << "," <<
           action.action << "," <<
           x.second << std::endl;
    }
    qt.close();
}


int main() {
    const std::deque<char> default_deck = {'A', 'A', 'A', 'A', '2', '2', '2', '2', '3', '3', '3', '3',
                                     '4', '4', '4', '4', '5', '5', '5', '5', '6', '6', '6', '6',
                                     '7', '7', '7', '7', '8', '8', '8', '8', '9', '9', '9', '9',
                                     '1', '1', '1', '1', 'J', 'J', 'J', 'J', 'Q', 'Q', 'Q', 'Q', 'K', 'K', 'K', 'K'};
    int initial_number_cards = 2;
    int winning_points = 21;
    int dealer_criticial_points_stick = 17;
    DefaultDict<StateAction, double> qtable_QL = training::train(1000000, 100000,
                                                              "Q-learning",
                                                              default_deck,
                                                              initial_number_cards,
                                                              winning_points,
                                                              dealer_criticial_points_stick);
    send_to_file("ql", qtable_QL);
    std::cout << "Done QL!" << std::endl;

    DefaultDict<StateAction, double> qtable_Sarsa = training::train(1000000, 100000,
                                                                 "Sarsa",
                                                                 default_deck,
                                                                 initial_number_cards,
                                                                 winning_points,
                                                                 dealer_criticial_points_stick);
    send_to_file("sarsa", qtable_Sarsa);
    std::cout << "Done Sarsa!" << std::endl;

    DefaultDict<StateAction, double> qtable_TD = training::train(1000000, 100000,
                                                                    "TD",
                                                                    default_deck,
                                                                    initial_number_cards,
                                                                    winning_points,
                                                                    dealer_criticial_points_stick);
    send_to_file("td", qtable_TD);
    std::cout << "Done TD!" << std::endl;

    return 0;
}
