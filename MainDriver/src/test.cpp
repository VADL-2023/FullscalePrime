#include <iostream>
#include "Root.h"
#include "State.h"
#include "State_Stepper_Test.h"
#include "State_Initial.h"
#include "State_Final.h"
#include <map>
#include "pigpio.h"
int main() {
    Root root;

    //State Definitions and Declarations

    //State Initial
    StateName initial_name = STATE_INITIAL;
    std::map<EventName,StateName> initial_transitions;
    initial_transitions.insert(std::pair<EventName,StateName>(INITIALIZE,STATE_STEPPER));
    State_Initial initial(initial_name,initial_transitions,&root);

    //State Stepper Test
    StateName stepper_name = STATE_STEPPER;
    std::map<EventName,StateName> stepper_transitions;
    stepper_transitions.insert(std::pair<EventName,StateName>(BASIC_SWIVEL,STATE_FINAL));
    State_Stepper_Test stepper_test(stepper_name,stepper_transitions,&root);

    //State Final
    StateName final_name = STATE_FINAL;
    std::map<EventName,StateName> final_transitions;
    final_transitions.insert(std::pair<EventName,StateName>(TERMINATE,END_STATE));
    State_Final final(final_name,final_transitions,&root);

    
    //Add States to Machine
    root.addState(&initial);
    root.addState(&stepper_test);
    root.addState(&final);
    State* current_state = &initial;

    root.start_time_ = root.getCurrentTime();
    std::cout << "Start machine" << std::endl;
    EventName curr_event = current_state->execute();
    StateName next_state = current_state->getNextState(curr_event);
    while(next_state != END_STATE) {
        current_state = root.states_[next_state];
        curr_event = current_state->execute();
        next_state = current_state->getNextState(curr_event);
    }

    //Temporary termination thing
    std::cout << "State Machine Completed" << std::endl;
    return 0;
}