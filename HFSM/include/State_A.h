#ifndef STATE_A_H
#define STATE_A_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_A : public State {
    public:
        State_A();
        State_A(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
        EventName execute() override;
    private:
};
#endif