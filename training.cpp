//
// Created by liya on 1/26/20.
//

#include "training.h"

namespace training {
    std::unordered_map<Blackjack_state_action, double> train_test(int training_iteration, int testing_iteration,
                                                                  std::string method, std::vector<char> &deck_content,
                                                                  int initial_number_cards, int winning_points,
                                                                  int dealer_criticial_points_to_stick) {
        // check if input parameters are valid
        if (initial_number_cards * 10 > winning_points || initial_number_cards < 1) {
            std::cout << "invalid number of card points" << std::endl;

        }
        if (dealer_criticial_points_to_stick > winning_points) {
            std::cout << "dealer will hit until bust" << std::endl;
        }

        int test_result[] = {0, 0, 0};
        std::unordered_map<Blackjack_state_action, double> qtable;
        std::unordered_map<Blackjack_state, int> state_count;
        std::unordered_map<Blackjack_state_action, int> state_action_count;
        for (int i = 0; i < testing_iteration + training_iteration; i++) {
            // hit = 0, stick = 1
            int action = INT_MIN;
            int reward = INT_MIN;
            Blackjack game = Blackjack(deck_content, initial_number_cards, winning_points);
            std::vector<Blackjack_state_action> occured_state_actions;

            while (!game.finish) {
                if (action != 1) {
                    if (i < training_iteration) {
                        Blackjack_state state(game.player_points,
                                              game.dealer_first_card_points,
                                              game.player_usable_ace);
                        int count = (state_count.find(state) == state_count.end()) ? 0 : state_count.at(state);
                        double epsilon = 100 / double(100 + count);
                        action = strategy::epsilon_greedy(epsilon, qtable, game.player_points,
                                                          game.dealer_first_card_points, game.player_usable_ace);
                    } else {
                        action = strategy::best_action(qtable, game.player_points,game.dealer_first_card_points,
                                                       game.player_usable_ace);
                    }
                }

                Blackjack_state_action occured_state_action = Blackjack_state_action(game.player_points,
                                                                                     game.dealer_first_card_points,
                                                                                     game.player_usable_ace, action);
                if (std::find(occured_state_actions.begin(), occured_state_actions.end(), occured_state_action)
                    == occured_state_actions.end() && game.player_points <= winning_points) {
                    occured_state_actions.push_back(occured_state_action);
                }
                episode e = game_proceed(game, action, winning_points, dealer_criticial_points_to_stick);
                reward = e.reward;
                if (reward != INT_MIN) {
                    game.finish = true;
                }

            }
            if (i < training_iteration) {
                strategy::update_qtable(reward, occured_state_actions, qtable, state_count,
                                        state_action_count, method);
            } else {
                if (reward == 1) {
                    test_result[0] += 1;
                } else if (reward == -1) {
                    test_result[2] += 1;
                } else {
                    test_result[1] += 1;
                }
            }
        }

        std::cout << "after training for " << training_iteration << " iterations using " + method << std::endl;
        std::cout << "our program fights against dealer for " << testing_iteration << " rounds" << std::endl;
        std::cout << "Win: " << (double(test_result[0]) / testing_iteration) * 100.0 << "%" << std::endl;
        std::cout << "Draw: " << (double(test_result[1]) / testing_iteration) * 100.0 << "%" << std::endl;
        std::cout << "Lose: " << (double(test_result[2]) / testing_iteration) * 100.0 << "%" << std::endl;
        return qtable;
    }
}