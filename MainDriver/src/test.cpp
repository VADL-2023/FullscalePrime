#include <iostream>
#include "Root.h"
#include "State.h"
#include "State_Stepper_Test.h"
#include <map>
#include "pigpio.h"
int main() {
    Root root;

    //State Definitions and Declarations

    //State Stepper Test
    StateName stepper_name = STATE_STEPPER;
    std::map<EventName,StateName> stepper_transitions;
    stepper_transitions.insert(std::pair<EventName,StateName>(BASIC_SWIVEL,END_STATE));
    State_Stepper_Test stepper_test(stepper_name,stepper_transitions,&root);

    
    //Add States to Machine
    root.addState(&stepper_test);
    State* current_state = &stepper_test;

    root.start_time_ = root.getCurrentTime();
    std::cout << "Start machine" << std::endl;
    // Temporary initialization thing
    while(gpioInitialise() <= 0) {
        std::cout << "Not initialized" << std::endl;
    }
    std::cout << "Initialized" << std::endl;
    EventName curr_event = current_state->execute();
    StateName next_state = current_state->getNextState(curr_event);
    while(next_state != END_STATE && curr_event != TERMINATE) {
        current_state = root.states_[next_state];
        curr_event = current_state->execute();
        next_state = current_state->getNextState(curr_event);
    }

    //Temporary termination thing
    std::cout << "State Machine Completed" << std::endl;
    gpioTerminate();
    return 0;
}