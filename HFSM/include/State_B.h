#ifndef STATE_B_H
#define STATE_B_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_B : public State {
    public:
        State_B();
        State_B(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
        EventName execute() override;
    private:
};
#endif