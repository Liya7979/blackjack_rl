#include "../include/State.h"

// State
State::State(int player_points, int dealer_points, bool usable_ace) :
        player_points(player_points),
        dealer_points(dealer_points),
        usable_ace(usable_ace) {}

bool State::operator==(const State &other) const {
    return player_points == other.player_points &&
           dealer_points == other.dealer_points &&
           usable_ace == other.usable_ace;
}

int State::hash() const {
    return 100 * player_points + 10 * dealer_points + usable_ace;
}

// StateAction
StateAction::StateAction(const State &state, int action) :
        state(state),
        action(action) {}

bool StateAction::operator==(const StateAction &other) const {
    return state == other.state &&
           action == other.action;
}

int StateAction::hash() const {
    return 10 * state.hash() + action;
}

State StateAction::get_state() {
    State s(this->state.player_points, this->state.dealer_points, this->state.usable_ace);
    return s;
}