#ifndef STATE_H
#define STATE_H

#include <cstddef>
#include <functional>

struct State {
    int player_points, dealer_points;
    bool usable_ace;

    State() = default;

    State(int player_points, int dealer_points, bool usable_ace);

    bool operator==(const State &other) const;

    [[nodiscard]] int hash() const;
};

struct StateAction {
    State state;
    int action;

    StateAction() = default;

    StateAction(const State &state, int action);

    bool operator==(const StateAction &other) const;

    [[nodiscard]] int hash() const;

    State get_state();
};

namespace std {
    template<>
    struct hash<State> {
        std::size_t operator()(const State &state) const {
            return hash<int>()(state.hash());
        }
    };

    template<>
    struct hash<StateAction> {
        std::size_t operator()(const StateAction &state_action) const {
            return hash<int>()(state_action.hash());
        }
    };
}

#endif //STATE_H
