#include <fstream>
#include "../include/training.h"

double mean(const std::vector<double> &vec) {
    double sum = 0.0;
    for (double d: vec) {
        sum += d;
    }
    return sum / vec.size();
}

void send_additional(std::vector<double> &v, const std::string &method, const std::string& param) {
    std::ofstream ls;
    std::string name = method + "_" + param + ".txt";
    ls.open(name);
    for (auto &x: v) {
        ls << x << std::endl;
    }
    ls.close();
}

DefaultDict<StateAction, double>
training::train(int training_iter, int testing_iter, const std::string &method, std::deque<char> deck,
                int init_cards_num, int winning_points, int dealer_crit_points_to_stick) {
    std::vector<double> epsilons(100000), round_epsilons(100000);
    std::array<int, 3> results{0, 0, 0};
    std::vector<double> average_rewards;
    std::vector<double> score;
    /* hashmap with initial values of 0: key = player card points, dealer showing card,
     * usable ace (not usable=0, usable=1), action (hit=0, stick=1);
     * value = Q(s,a) function that stores the Q value for all combinations of states and actions */
    DefaultDict<StateAction, double> qtable;
    /* hashmap with initial values of 0: key = player card points, dealer showing card,
     * usable ace (not usable=0, usable=1), action (hit=0, stick=1);
     * value = frequency of some particular state-action pairs (used for calculating the step size */
    DefaultDict<State, int> state_count;
    /* hashmap with initial values of 0: key = player card points, dealer showing card,
     * usable ace (not usable=0, usable=1), action (hit=0, stick=1);
     * value = frequency of some particular states (used for calculating the epsilon decay) */
    DefaultDict<StateAction, int> state_action_count;
    int episodes = 0;
    int iterations = training_iter + testing_iter;
    // training and testing is performed
    for (int i = 0; i < iterations; ++i) {
        ++episodes;
        if (i % 100000 == 0) {
            std::cout << "Iterations passed: " << i << std::endl;
        }
        int rounds = 0;
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
            ++rounds;
            State state(game.player_points, game.dealer_first_card_points, game.player_usable_ace);
            StateAction state_action(state, action);
            if (!vec_contains(occurred_state_actions, state_action) && game.player_points <= winning_points) {
                occurred_state_actions.push_back(state_action);
            }
            // starting the game with the player's action
            Round round = step(game, action, dealer_crit_points_to_stick);
            game.player_points = round.player;
            game.dealer_points = round.dealer;
            game.player_usable_ace = round.usable_ace;
            reward = round.reward;
            // if we get a reward, the game is finished
            if (reward != -2) {
                if (i < training_iter) {
                    score.push_back(double(reward) / rounds);
                }
                epsilons.push_back(mean(round_epsilons));
                game.finished = true;

            }
        }
        if (i < training_iter) {
            // average reward calculated per each 1000 episode
            if (episodes % 1000 == 0) {
                average_rewards.push_back(mean(score));
                score.clear();
            }
            Strategy::update_qtable(reward, occurred_state_actions, qtable, state_count, state_action_count, method);
        } else {
            // testing phase: calculating the number of wins, draws and losses
            if (reward == 1) {
                ++results[0];
            } else if (reward == -1) {
                ++results[2];
            } else {
                ++results[1];
            }
        }
    }
    std::vector<double> performance(3);
    std::cout << "After training for " << training_iter << " episodes using " << method << std::endl;
    std::cout << "The system plays for " << testing_iter << " rounds" << std::endl;
    std::cout << "Win: " << (static_cast<double>(results[0]) / testing_iter * 100.0) << "%" << std::endl;
    std::cout << "Draw: " << (static_cast<double>(results[1]) / testing_iter * 100.0) << "%" << std::endl;
    std::cout << "Lose: " << (static_cast<double>(results[2]) / testing_iter * 100.0) << "%\n" << std::endl;
    performance.push_back((static_cast<double>(results[0]) / testing_iter * 100.0));
    performance.push_back((static_cast<double>(results[1]) / testing_iter * 100.0));
    performance.push_back((static_cast<double>(results[2]) / testing_iter * 100.0));
    // sending the results for post-processsing with Python
    send_additional(average_rewards, method, "rewards");
    send_additional(epsilons, method, "eps");
    send_additional(performance, method, "performance");
    return qtable;
}
