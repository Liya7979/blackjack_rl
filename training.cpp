//
// Created by liya on 1/26/20.
//

#include "training.h"

namespace training {
    void test_system(std::unordered_map<State_action, double> qtable, int training_iteration,
                     int testing_iteration,
                     const std::string &method, std::vector<char> &deck_content,
                     int initial_number_cards, int winning_points,
                     int dealer_criticial_points_to_stick) {
        int test_result[] = {0, 0, 0};
        int score = 0;
        for (int i = 0; i < testing_iteration; i++) {
            int action;
            int reward = INT_MIN;
            Blackjack game = Blackjack(deck_content, initial_number_cards, winning_points);
            while (!game.finish) {
                action = strategy::best_action(qtable, game.player_points, game.dealer_first_card_points,
                                               game.player_usable_ace);
                Episode e = game_proceed(game, action, winning_points, dealer_criticial_points_to_stick);
                reward = e.reward;
                if (e.done) {
                    game.finish = true;
                }
            }
            if (reward == 1) {
                test_result[0] += 1;
            } else if (reward == -1) {
                test_result[2] += 1;
            } else {
                test_result[1] += 1;
            }
        }
        std::cout << "after training for " << training_iteration << " iterations using " + method << std::endl;
        std::cout << "our program fights against dealer for " << testing_iteration << " rounds" << std::endl;
        std::cout << "Win: " << (double(test_result[0]) / testing_iteration) * 100.0 << "%" << std::endl;
        std::cout << "Draw: " << (double(test_result[1]) / testing_iteration) * 100.0 << "%" << std::endl;
        std::cout << "Lose: " << (double(test_result[2]) / testing_iteration) * 100.0 << "%" << std::endl;
    }

    std::unordered_map<State_action, double> train(int training_iteration, int testing_iteration,
                                                   const std::string &method, std::vector<char> &deck_content,
                                                   int initial_number_cards, int winning_points,
                                                   int dealer_criticial_points_to_stick) {
        // check if input parameters are valid
        if (initial_number_cards * 10 > winning_points || initial_number_cards < 1) {
            std::cout << "invalid number of card points" << std::endl;
        }
        if (dealer_criticial_points_to_stick > winning_points) {
            std::cout << "dealer will hit until bust" << std::endl;
        }
        std::unordered_map<State_action, double> qtable;
        std::unordered_map<State, int> state_count;
        std::unordered_map<State_action, int> state_action_count;
        for (int i = 1; i < training_iteration + 1; i++) {
            int score = 0;
            // hit = 0, stick = 1
            int action;
            int reward;
            Blackjack game = Blackjack(deck_content, initial_number_cards, winning_points);
            State state = reset_game(game);
            double epsilon = std::max(1.0 / i, 0.01);
            while (!game.finish) {
                action = strategy::epsilon_greedy(epsilon, qtable, game.player_points,
                                                  game.dealer_first_card_points, game.player_usable_ace);
                Episode e = game_proceed(game, action, winning_points, dealer_criticial_points_to_stick);
                reward = e.reward;
                score += reward;
                strategy::update_qtable(e, action, state, qtable, method);
                state = State(e.player, e.dealer, e.reward);
                if (e.done) {
                    game.finish = true;
                }
            }


        }
        test_system(qtable, training_iteration, testing_iteration, method, deck_content, initial_number_cards,
                    winning_points, dealer_criticial_points_to_stick);

        return qtable;
    }

}
