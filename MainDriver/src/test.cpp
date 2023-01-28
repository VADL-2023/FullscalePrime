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
    std::string stepper_name = "state_stepper";
    std::map<std::string,std::string> stepper_transitions;
    stepper_transitions.insert(std::pair<std::string,std::string>("swivel_stepper","end_state"));
    State_Stepper_Test stepper_test(stepper_name,stepper_transitions,&root);

    
    //Add States to Machine
    root.addStates(&stepper_test);
    State* current_state = &stepper_test;

    root.start_time_ = root.getCurrentTime();
    std::cout << "Start machine" << std::endl;
    // Temporary initialization thing
    while(gpioInitialise() <= 0) {
        std::cout << "Not initialized" << std::endl;
    }
    std::cout << "Initialized" << std::endl;
    std::string next_state_str = current_state->execute();
    std::cout << "Next state: " << next_state_str << std::endl;
    while(next_state_str != "end_state") {
        current_state = root.states_[next_state_str];
        next_state_str = current_state->execute();
        std::cout << "Next state: " << next_state_str << std::endl;
    }

    //Temporary termination thing
    std::cout << "State Machine Completed" << std::endl;
    gpioTerminate();
    return 0;
}