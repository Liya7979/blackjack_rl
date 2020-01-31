#include "../include/Strategy.h"

double Strategy::random_between_0_1() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    return dis(gen);
}

int Strategy::random_action() {
    return random_between_0_1() <= 0.5;
}

int Strategy::epsilon_greedy(double epsilon, DefaultDict<StateAction, double> &qtable, const State &state) {
    if (random_between_0_1() < epsilon) {
        return random_action();
    } else {
        return best_action(qtable, state);
    }
}

int Strategy::best_action(DefaultDict<StateAction, double> &qtable, const State &state) {
    StateAction hit(state, 0), stick(state, 1);
    double
            hit_value = qtable.at(hit),
            stick_value = qtable.at(stick);
    if (hit_value > stick_value) {
        return 0;
    } else if (hit_value < stick_value) {
        return 1;
    } else {
        return random_action();
    }
}

void Strategy::update_qtable(int reward, std::vector<StateAction> &occurred_state_actions, DefaultDict<StateAction,
        double> &qtable, DefaultDict<State, int> &state_count, DefaultDict<StateAction, int> &state_action_count,
                             const std::string &method, double gamma) {
    for (int i = 0; i < occurred_state_actions.size(); ++i) {
        StateAction state_action = occurred_state_actions.at(i);
        State state = state_action.get_state();

        ++state_count.at(state);
        ++state_action_count.at(state_action);

        double alpha = 1.0 / state_action_count.at(state_action);

        if (method == "Q_learning" || method == "Sarsa") {
            double previous_Q = qtable.at(state_action);
            double best_next_q;
            if (i < occurred_state_actions.size() - 1) {
                if (method == "Q_learning") {
                    State next_state = occurred_state_actions.at(i + 1).get_state();
                    StateAction
                            hit(next_state, 0),
                            stick(next_state, 1);
                    best_next_q = gamma * std::max(qtable.at(hit), qtable.at(stick));
                } else {
                    // Sarsa
                    StateAction next_state_action = occurred_state_actions.at(i + 1);
                    best_next_q = gamma * qtable.at(next_state_action);
                }

            } else {
                best_next_q = 0;
            }
            StateAction occurred = occurred_state_actions.at(i);
            // the reward is the same for all occured state-action pairs since it is the outcome of after taking
            // these actions in particular state
            qtable.at(occurred) = (1 - alpha) * previous_Q + alpha * (reward + best_next_q);
        } else {
            // TD
            StateAction occurred = occurred_state_actions.at(i);
            qtable.at(occurred) += alpha * (reward - qtable.at(occurred));
        }
    }
}


