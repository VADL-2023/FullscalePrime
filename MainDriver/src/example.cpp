#include <iostream>
#include "Root.h"
#include "State.h"
#include "State_A.h"
#include "State_B.h"
#include <map>

int main() {
    Root root;

    //State Definitions and Declarations

    //State A
    StateName a_name = STATE_A;
    std::map<EventName, StateName> a_transitions;
    a_transitions.insert(std::pair<EventName, StateName>(TIME_10, STATE_B));
    a_transitions.insert(std::pair<EventName, StateName>(TIME_20, END_STATE));
    State_A a(a_name,a_transitions,&root);
   

    //State B
    StateName b_name = STATE_B;
    std::map<EventName, StateName> b_transitions;
    b_transitions.insert(std::pair<EventName, StateName>(TIME_15, STATE_A));
    State_B b(b_name,b_transitions,&root);

    
    //Add States to Machine
    root.addState(&a);
    root.addState(&b);
    State* curr_state = &a;

    root.start_time_ = root.getCurrentTime();
    std::cout << "Start machine" << std::endl;
    EventName curr_event = curr_state->execute();
    StateName next_state = curr_state->getNextState(curr_event);
    while(next_state != END_STATE && curr_event != TERMINATE) {
        curr_state = root.states_[next_state];
        curr_event = curr_state->execute();
        next_state = curr_state->getNextState(curr_event);
    }

    std::cout << "State Machine Completed" << std::endl;

    return 0;
}