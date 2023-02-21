#ifndef STATE_H
#define STATE_H

#include "State_Enums.h"

#include <iostream>
#include <map>

class Root;

class State {
    public:
        State();
        State(StateName &name, std::map<EventName, StateName> &state_transitions, Root* root);
        virtual EventName execute();
        virtual EventName unitExecute();
        StateName getName();
        StateName getNextState(EventName curr_event);

    // Subclasses cannot access these variables unless they protected, not private 
    protected:
        Root* root_;
        StateName name_;
        std::map<EventName,StateName> state_transitions_;
};

#endif