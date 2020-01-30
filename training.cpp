#include "training.h"

double mean(const std::vector<double> &vec) {
    double sum = 0.0;
    for (double d: vec) {
        sum += d;
    }
    return sum / vec.size();
}

DefaultDict<StateAction, double>
training::train(int training_iter, int testing_iter, const std::string &method, std::deque<char> deck,
                int init_cards_num, int winning_points, int dealer_crit_points_to_stick) {
    std::vector<double> epsilons(100000), round_epsilons(100000);
    std::array<int, 3> results{0, 0, 0};

    DefaultDict<StateAction, double> qtable;
    DefaultDict<State, int> state_count;
    DefaultDict<StateAction, int> state_action_count;

    int iterations = training_iter + testing_iter;
    for (int i = 0; i < iterations; ++i) {
        if (i % 10000 == 0) {
            std::cout << "Iterations passed: " << i << std::endl;
        }
        int action = -1; // no action
        int reward = -2; // since -1, 0 and 1 are used

        Blackjack game(deck, init_cards_num, winning_points);
        std::vector<StateAction> occurred_state_actions;

        while (!game.finished) {
            if (action != 1) {
                State state(game.player_points, game.dealer_first_card_points, game.player_usable_ace);
                if (i < training_iter) {
                    double epsilon = 100.0 / (100.0 + state_count.at(state));
                    action = Strategy::epsilon_greedy(epsilon, qtable, state);
                    round_epsilons.push_back(epsilon);
                } else {
                    action = Strategy::best_action(qtable, state);
                }
            }
            State state(game.player_points, game.dealer_first_card_points, game.player_usable_ace);
            StateAction state_action(state, action);
            if (!vec_contains(occurred_state_actions, state_action) && game.player_points <= winning_points) {
                occurred_state_actions.push_back(state_action);
            }
            Round round = step(game, action, dealer_crit_points_to_stick);
            game.player_points = round.player;
            game.dealer_points = round.dealer;
            game.player_usable_ace = round.usable_ace;
            reward = round.reward;
            if (reward != -2) {
                game.finished = true;
                epsilons.push_back(mean(round_epsilons));
            }
        }
        if (i < training_iter) {
            Strategy::update_qtable(reward, occurred_state_actions, qtable, state_count, state_action_count, method);
        } else {
            if (reward == 1) {
                ++results[0];
            } else if (reward == -1) {
                ++results[2];
            } else {
                ++results[1];
            }
        }
    }
    std::cout << "After training for " << training_iter << " iterations with " << method << std::endl;
    std::cout << "Our system plays for " << testing_iter << " rounds" << std::endl;
    std::cout << "Win: " << (static_cast<double>(results[0]) / testing_iter * 100.0) << "%" << std::endl;
    std::cout << "Draw: " << (static_cast<double>(results[1]) / testing_iter * 100.0) << "%" << std::endl;
    std::cout << "Lose: " << (static_cast<double>(results[2]) / testing_iter * 100.0) << "%\n" << std::endl;
    return qtable;
}
